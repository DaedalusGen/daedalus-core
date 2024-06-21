#include <AquIce/daedalus/lexer.hpp>
#include <AquIce/daedalus/parser.hpp>
#include <AquIce/daedalus/interpreter.hpp>

#include <functional>

namespace daedalus {
	typedef struct Daedalus {
		daedalus::lexer::Lexer lexer;
		daedalus::parser::Parser parser;
		daedalus::interpreter::Interpreter interpreter;
	} Daedalus;

	typedef std::function<void (daedalus::lexer::Lexer& lexer)> LexerConfigFunction;
	typedef std::function<void (daedalus::parser::Parser& parser)> ParserConfigFunction;
	typedef std::function<void (daedalus::interpreter::Interpreter& interpreter)> InterpreterConfigFunction;

	Daedalus setup_daedalus(
		LexerConfigFunction lexerConfigFunction,
		ParserConfigFunction parserConfigFunction,
		InterpreterConfigFunction interpreterConfigFunction
	);
}