#ifndef __DAEDALUS_CORE_INTERPRETER__
#define __DAEDALUS_CORE_INTERPRETER__

#include <daedalus/core/parser/ast.hpp>
#include <daedalus/core/interpreter/values.hpp>
#include <daedalus/core/interpreter/env.hpp>
#include <daedalus/core/tools/assert.hpp>

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace daedalus {
    namespace core {
    	namespace interpreter {

            typedef size_t Flags;

            typedef struct RuntimeValueWrapper {
                std::shared_ptr<daedalus::core::values::RuntimeValue> value;
                Flags flags;
                bool returnStatementBefore;
            } RuntimeValueWrapper;

            RuntimeValueWrapper wrap(
                std::shared_ptr<daedalus::core::values::RuntimeValue> value,
                size_t flags = 0,
                bool returnStatementBefore = false
            );

            bool flag_contains(Flags source, Flags searched);
            Flags flag_remove(Flags source, Flags to_remove);

    		struct Interpreter;

    		typedef std::function<daedalus::core::interpreter::RuntimeValueWrapper (
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

            typedef struct RuntimeResult {
                std::string expressionRepr;
                std::string valueRepr;
            } RuntimeResult;

    		RuntimeValueWrapper evaluate_statement(
    			Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);

    		RuntimeValueWrapper evaluate_scope(
    			Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Scope> scope,
    			std::vector<RuntimeResult>& results,
    			std::shared_ptr<daedalus::core::env::Environment> scope_env = nullptr,
    			std::shared_ptr<daedalus::core::env::Environment> parent_env = nullptr,
                Flags escape_flag = 0
    		);

    		void interpret(
    			Interpreter& interpreter,
    			std::vector<RuntimeResult>& results,
    			std::shared_ptr<daedalus::core::ast::Scope> program
    		);
    	}
    }
}

#endif // __DAEDALUS_CORE_INTERPRETER__
