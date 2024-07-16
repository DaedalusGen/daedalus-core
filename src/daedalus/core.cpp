#include <AquIce/daedalus/core.hpp>

daedalus::core::Daedalus daedalus::core::setup_daedalus(
	daedalus::core::LexerConfigFunction lexerConfigFunction,
	daedalus::core::ParserConfigFunction parserConfigFunction,
	daedalus::core::InterpreterConfigFunction interpreterConfigFunction
) {
	daedalus::core::lexer::Lexer lexer;
	daedalus::core::parser::Parser parser;
	daedalus::core::interpreter::Interpreter interpreter;

	lexerConfigFunction(lexer);
	parserConfigFunction(parser);
	interpreterConfigFunction(interpreter);

	return daedalus::core::Daedalus{ lexer, parser, interpreter };
}
