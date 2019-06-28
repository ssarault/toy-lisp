(define (power x n) (let ((fn (lambda (c v n f) (if (= n 1) c (f (* c v) v (- n 1) f))))) (fn x x n fn)))

(defmacro (defstruct name . fields)
  (let ((names (map (lambda (symbol) (gensym)) fields))
        (struct (gensym))
        (field-arg (gensym)))
    `(if (not (every-unique ',fields))
         (error 'defstruct "Fields must be unique")
         (begin
           (define (,(make-name name) ,@names)
             (map cons ',fields (list ,@names)))
           ,@(map (lambda (field)
                    `(define (,(make-getter name field) ,struct)
                       (cdr (assq ',field ,struct)))) fields)
           ,@(map (lambda (field)
                    `(define (,(make-setter name field) ,struct ,field-arg)
                       (assq-set! ,struct ',field ,field-arg)
                       ,field-arg)) fields)
           (define (,(make-predicate name) ,struct)
             (and (struct? ,struct)
                  (let ((result #t))
                    (for-each (lambda (x y)
                                (if (not (eq? x y)) (set! result #f)))
                              ',fields
                              (map car ,struct))
                    result)))))))