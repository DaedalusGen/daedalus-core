#include <AquIce/daedalus/core.hpp>

daedalus::Daedalus daedalus::setup_daedalus(
	daedalus::LexerConfigFunction lexerConfigFunction,
	daedalus::ParserConfigFunction parserConfigFunction,
	daedalus::InterpreterConfigFunction interpreterConfigFunction
) {
	daedalus::lexer::Lexer lexer;
	daedalus::parser::Parser parser;
	daedalus::interpreter::Interpreter interpreter;

	lexerConfigFunction(lexer);
	parserConfigFunction(parser);
	interpreterConfigFunction(interpreter);

	return daedalus::Daedalus{ lexer, parser, interpreter };
}