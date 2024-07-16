#ifndef __DAEDALUS_CORE_CORE__
#define __DAEDALUS_CORE_CORE__

#include <daedalus/core/lexer/lexer.hpp>
#include <daedalus/core/parser/parser.hpp>
#include <daedalus/core/interpreter/interpreter.hpp>

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

#endif // __DAEDALUS_CORE_CORE__
