#include <AquIce/daedalus/interpreter.hpp>

void daedalus::interpreter::setup_interpreter(
	daedalus::interpreter::Interpreter& interpreter,
	std::unordered_map<std::string, ParseStatementFunction> nodeEvaluationFunctions,
	std::vector<std::string> envValuesProperties,
	std::vector<daedalus::env::EnvValidationRule> validationRules
) {
	interpreter.envValuesProperties = envValuesProperties;

	interpreter.validationRules = validationRules;

	interpreter.nodeEvaluationFunctions = nodeEvaluationFunctions;
	interpreter.nodeEvaluationFunctions["NumberExpression"] = [] (
		daedalus::interpreter::Interpreter& interpreter,
		std::shared_ptr<daedalus::ast::Statement> statement,
		std::shared_ptr<daedalus::env::Environment> env
	) -> std::shared_ptr<daedalus::values::RuntimeValue> {
		return std::make_shared<daedalus::values::NumberValue>(
			std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(statement)->value
		);
	};
}

std::shared_ptr<daedalus::values::RuntimeValue> daedalus::interpreter::evaluate_statement(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
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

std::shared_ptr<daedalus::values::RuntimeValue> daedalus::interpreter::evaluate_scope(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Scope> scope,
	std::unordered_map<std::string, std::string>& results,
	std::shared_ptr<daedalus::env::Environment> scope_env,
	std::shared_ptr<daedalus::env::Environment> parent_env
) {
	if(scope_env == nullptr) {
		scope_env = std::make_shared<daedalus::env::Environment>(
			interpreter.envValuesProperties,
			interpreter.validationRules,
			parent_env
		);
	}

	std::shared_ptr<daedalus::values::RuntimeValue> result;
	std::shared_ptr<daedalus::values::RuntimeValue> prev_result = std::make_shared<daedalus::values::NullValue>();

	for(std::shared_ptr<daedalus::ast::Statement> statement : scope->body) {
		result = daedalus::interpreter::evaluate_statement(interpreter, statement, scope_env);
		prev_result = result;
		results[statement->repr()] = result->repr();
	}
	return result;
}

void daedalus::interpreter::interpret(
	daedalus::interpreter::Interpreter& interpreter,
	std::unordered_map<std::string, std::string>& results,
	std::shared_ptr<daedalus::ast::Scope> program
) {
	std::shared_ptr<daedalus::env::Environment> env = std::make_shared<daedalus::env::Environment>(
		interpreter.envValuesProperties,
		interpreter.validationRules
	);

	daedalus::interpreter::evaluate_scope(
		interpreter,
		program,
		results,
		env
	);
}