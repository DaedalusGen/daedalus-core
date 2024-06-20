.ONESHELL:

default: run

build_obj:
	cd build
	g++ -I../include -Wall -Wpedantic -Werror ../daedalus/lexer/lexer.cpp ../daedalus/parser/ast.cpp ../daedalus/parser/parser.cpp ../daedalus/interpreter/values.cpp ../daedalus/interpreter/env.cpp ../daedalus/interpreter/interpreter.cpp -c
	cd ..

build_lib: build_obj
	cd build
	g++ -shared -o ../daedalus.dll *.o
	cd ..

build_wlib: build_lib
	g++ -Iinclude -Wall -Wpedantic -Werror daedalus.dll main.cpp -o lg

build_for_run:
	g++ -Iinclude -Wall -Wpedantic -Werror daedalus/lexer/lexer.cpp daedalus/parser/ast.cpp daedalus/parser/parser.cpp daedalus/interpreter/values.cpp daedalus/interpreter/env.cpp daedalus/interpreter/interpreter.cpp main.cpp -o lg

run: build_for_run
	./lg