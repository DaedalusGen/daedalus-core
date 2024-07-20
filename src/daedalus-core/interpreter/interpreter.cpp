#include <daedalus/core/interpreter/interpreter.hpp>

daedalus::core::interpreter::RuntimeValueWrapper daedalus::core::interpreter::wrap(
    std::shared_ptr<daedalus::core::values::RuntimeValue> value,
    Flags flags,
    bool returnStatementBefore
) {
    return daedalus::core::interpreter::RuntimeValueWrapper{
        value,
        flags,
        returnStatementBefore
    };
}

bool daedalus::core::interpreter::flag_contains(
    daedalus::core::interpreter::Flags source,
    daedalus::core::interpreter::Flags searched
) {
    return (static_cast<size_t>(source) & static_cast<size_t>(searched)) != 0;
}
daedalus::core::interpreter::Flags daedalus::core::interpreter::flag_remove(
    daedalus::core::interpreter::Flags source,
    daedalus::core::interpreter::Flags to_remove
) {
    return static_cast<daedalus::core::interpreter::Flags>(static_cast<size_t>(source) & ~static_cast<size_t>(to_remove));
}

void daedalus::core::interpreter::setup_interpreter(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::unordered_map<std::string, ParseStatementFunction> nodeEvaluationFunctions,
	std::vector<std::string> envValuesProperties,
	std::vector<daedalus::core::env::EnvValidationRule> validationRules
) {
	interpreter.envValuesProperties = envValuesProperties;

	interpreter.validationRules = validationRules;

	interpreter.nodeEvaluationFunctions = nodeEvaluationFunctions;
	interpreter.nodeEvaluationFunctions["NumberExpression"] = [] (
		daedalus::core::interpreter::Interpreter& interpreter,
		std::shared_ptr<daedalus::core::ast::Statement> statement,
		std::shared_ptr<daedalus::core::env::Environment> env
	) -> daedalus::core::interpreter::RuntimeValueWrapper {
	    return daedalus::core::interpreter::wrap(
			std::make_shared<daedalus::core::values::NumberValue>(
    			std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(statement)->value
    		)
		);
	};
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::core::interpreter::evaluate_statement(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	for(const auto& [nodeType, evaluateFn] : interpreter.nodeEvaluationFunctions) {
		if(statement->type() == nodeType) {
			return evaluateFn(interpreter, statement, env);
		}
	}

	DAE_ASSERT_TRUE(
		false,
		std::runtime_error("Trying to evaluate unknown statement " + statement->type())
	)
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::core::interpreter::evaluate_scope(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Scope> scope,
	std::vector<daedalus::core::interpreter::RuntimeResult>& results,
	std::shared_ptr<daedalus::core::env::Environment> scope_env,
	std::shared_ptr<daedalus::core::env::Environment> parent_env,
	Flags escape_flag
) {
	if(scope_env == nullptr) {
		scope_env = std::make_shared<daedalus::core::env::Environment>(
			interpreter.envValuesProperties,
			interpreter.validationRules,
			parent_env
		);
	}

	daedalus::core::interpreter::RuntimeValueWrapper result;
	daedalus::core::interpreter::RuntimeValueWrapper previous_result = daedalus::core::interpreter::wrap(
        std::make_shared<daedalus::core::values::NullValue>()
	);

	for(std::shared_ptr<daedalus::core::ast::Statement> statement : scope->body) {
		result = daedalus::core::interpreter::evaluate_statement(interpreter, statement, scope_env);
		if(daedalus::core::interpreter::flag_contains(result.flags, escape_flag)) {
			previous_result.flags = result.flags;
			return result.returnStatementBefore ? previous_result : result;
		}
		previous_result = result;
		results.push_back(daedalus::core::interpreter::RuntimeResult{
		    statement->repr(),
			result.value->repr()
		});
	}
	return result;
}

void daedalus::core::interpreter::interpret(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::vector<daedalus::core::interpreter::RuntimeResult>& results,
	std::shared_ptr<daedalus::core::ast::Scope> program
) {
	std::shared_ptr<daedalus::core::env::Environment> env = std::make_shared<daedalus::core::env::Environment>(
		interpreter.envValuesProperties,
		interpreter.validationRules
	);

	daedalus::core::interpreter::evaluate_scope(
		interpreter,
		program,
		results,
		env
	);
}
