
(define (abs x) (if (< x 0) (- 0 x) x))

(define (foldl proc init list)
  (if list
      (foldl proc
             (proc init (car list))
             (cdr list))
      init))

(define (foldr proc init list)
  (if list
      (proc (car list)
            (foldr proc init (cdr list)))
      init))

(define (list . items)
  (foldr cons nil items))

(define (reverse list)
  (foldl (lambda (a x) (cons x a)) nil list))

(define (unary-map proc list)
  (foldr (lambda (x rest) (cons (proc x) rest))
         nil
         list))

(define (map proc . arg-lists)
  (if (car arg-lists)
      (cons (apply proc (unary-map car arg-lists))
            (apply map (cons proc
                             (unary-map cdr arg-lists))))
      nil))

(define (append a b) (foldr cons b a))

(define (caar x) (car (car x)))

(define (cadr x) (car (cdr x)))

(defmacro (m-and a b) (list 'if a b nil))

(defmacro (quasiquote x)
  (if (pair? x)
      (if (eq? (car x) 'unquote)
          (cadr x)
          (if (m-and (pair? (car x)) (eq? (caar x) 'unquote-splicing))
              (list 'append
                    (cadr (car x))
                    (list 'quasiquote (cdr x)))
              (list 'cons
                    (list 'quasiquote (car x))
                    (list 'quasiquote (cdr x)))))
      (list 'quote x)))

(defmacro (let defs . body)
  `((lambda ,(map car defs) ,@body)
    ,@(map cadr defs)))

(define +
  (let ((old+ +))
    (lambda xs (foldl old+ 0 xs))))

(define concat
  (let ((oldconcat concat))
    (lambda xs (foldl oldconcat (car xs) (cdr xs)))))

(define (!= a b)
    (if (= a b)
        nil
    'T))

(define (not expr)
    (if (eq? expr nil)
        'T
    nil))

(define (>= a b)
    (if (> a b)
        'T
    (if (= a b)
        'T
    nil)))

(define (<= a b)
    (if (< a b)
        'T
    (if (= a b)
        'T
    nil)))

(define (or a b)
    (if (eq? a 'T)
      'T
    (if (eq? b 'T)
        'T
    nil)))

(define or
  (let ((oldor or))
    (lambda xs (foldl oldor nil xs))))

(define (and a b)
    (if (eq? a 'T)
      (if (eq? b 'T)
          'T
          nil)
      nil))

(define and (let ((oldand and)) (lambda xs (foldl oldand 'T xs))))

(define (power x n)
  (let ((fn (lambda (c v n f) (if (= n 1) c (f (* c v) v (- n 1) f)))))
    (fn x x n fn)))

(define ([] list index)
    (if (< index 0)
        nil
    (if (= index 0)
        (car list) 
    ([] (cdr list) (- index 1)))))

(define (make-name name)
  (concat 'make- name))

(define (make-getter name field)
  (concat name '- field))

(define (make-predicate name)
  (concat name '?))

(define (begin . items)
  (car (reverse items)))

(define (special-fn x)
    (begin
        (if (= x 0)
            'T
        nil)
    ))

(define (for-each l fn)
    (if (car list)
        (begin
            (fn (car list))
            (for-each (cdr list) fn))
        nil))