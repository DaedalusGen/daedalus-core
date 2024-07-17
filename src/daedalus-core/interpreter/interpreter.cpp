#include <daedalus/core/interpreter/interpreter.hpp>

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
	) -> std::shared_ptr<daedalus::core::values::RuntimeValue> {
		return std::make_shared<daedalus::core::values::NumberValue>(
			std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(statement)->value
		);
	};
}

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::core::interpreter::evaluate_statement(
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

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::core::interpreter::evaluate_scope(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Scope> scope,
	std::vector<daedalus::core::interpreter::RuntimeResult>& results,
	std::shared_ptr<daedalus::core::env::Environment> scope_env,
	std::shared_ptr<daedalus::core::env::Environment> parent_env
) {
	if(scope_env == nullptr) {
		scope_env = std::make_shared<daedalus::core::env::Environment>(
			interpreter.envValuesProperties,
			interpreter.validationRules,
			parent_env
		);
	}

	std::shared_ptr<daedalus::core::values::RuntimeValue> result;
	std::shared_ptr<daedalus::core::values::RuntimeValue> prev_result = std::make_shared<daedalus::core::values::NullValue>();

	for(std::shared_ptr<daedalus::core::ast::Statement> statement : scope->body) {
		result = daedalus::core::interpreter::evaluate_statement(interpreter, statement, scope_env);
		prev_result = result;
		results.push_back(daedalus::core::interpreter::RuntimeResult{
		    statement->repr(),
			result->repr()
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
