clean:
	rm -rf lisp

all:
	make clean
	gcc lisp.c lisp_environment.c lisp_builtins.c lisp_parser.c lisp_library.c main.c -o lisp -lreadline -g -Wall
