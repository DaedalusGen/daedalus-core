#include <AquIce/daedalus/lexer.hpp>
#include <AquIce/daedalus/parser.hpp>
#include <AquIce/daedalus/interpreter.hpp>

#include <functional>

namespace daedalus {
    namespace core {
        typedef struct Daedalus {
       		daedalus::core::lexer::Lexer lexer;
       		daedalus::core::parser::Parser parser;
       		daedalus::core::interpreter::Interpreter interpreter;
       	} Daedalus;

       	typedef std::function<void (daedalus::core::lexer::Lexer& lexer)> LexerConfigFunction;
       	typedef std::function<void (daedalus::core::parser::Parser& parser)> ParserConfigFunction;
       	typedef std::function<void (daedalus::core::interpreter::Interpreter& interpreter)> InterpreterConfigFunction;

       	Daedalus setup_daedalus(
       		LexerConfigFunction lexerConfigFunction,
       		ParserConfigFunction parserConfigFunction,
       		InterpreterConfigFunction interpreterConfigFunction
       	);
    }
}
