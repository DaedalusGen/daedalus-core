#ifndef __DAEDALUS_CORE_INTERPRETER__
#define __DAEDALUS_CORE_INTERPRETER__

#include <daedalus/core/parser/ast.hpp>
#include <daedalus/core/interpreter/values.hpp>
#include <daedalus/core/interpreter/env.hpp>
#include <daedalus/core/tools/assert.hpp>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace daedalus {
    namespace core {
    	namespace interpreter {

    		struct Interpreter;

    		typedef std::function<std::shared_ptr<daedalus::core::values::RuntimeValue> (
    			Interpreter&,
    			std::shared_ptr<daedalus::core::ast::Statement>,
    			std::shared_ptr<daedalus::core::env::Environment>
    		)> ParseStatementFunction;

    		typedef struct Interpreter {
    			std::unordered_map<std::string, ParseStatementFunction> nodeEvaluationFunctions;
    			std::vector<std::string> envValuesProperties;
    			std::vector<daedalus::core::env::EnvValidationRule> validationRules;
    		} Interpreter;

    		void setup_interpreter(
    			Interpreter& interpreter,
    			std::unordered_map<std::string, ParseStatementFunction> nodeEvaluationFunctions,
    			std::vector<std::string> envValuesProperties,
    			std::vector<daedalus::core::env::EnvValidationRule> validationRules
    		);

    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_statement(
    			Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);

    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_scope(
    			Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Scope> scope,
    			std::unordered_map<std::string, std::string>& results,
    			std::shared_ptr<daedalus::core::env::Environment> scope_env = nullptr,
    			std::shared_ptr<daedalus::core::env::Environment> parent_env = nullptr
    		);

    		void interpret(
    			Interpreter& interpreter,
    			std::unordered_map<std::string, std::string>& results,
    			std::shared_ptr<daedalus::core::ast::Scope> program
    		);
    	}
    }
}

#endif // __DAEDALUS_CORE_INTERPRETER__
