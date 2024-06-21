#include <iostream>
#include <string>
#include <vector>

#include <AquIce/daedalus/core.hpp>

#pragma region Prototypes

void setup_lexer(daedalus::lexer::Lexer& lexer);

std::shared_ptr<daedalus::ast::Expression> parse_multiplicative_expression(std::vector<daedalus::lexer::Token>& tokens);
std::shared_ptr<daedalus::ast::Expression> parse_additive_expression(std::vector<daedalus::lexer::Token>& tokens);
std::shared_ptr<daedalus::ast::Expression> parse_binary_expression(std::vector<daedalus::lexer::Token>& tokens);

void setup_parser(daedalus::parser::Parser& parser);

bool validate_mutability(daedalus::env::EnvValue value);

std::string get_mutability_error_message(std::string key, std::string on);

std::shared_ptr<daedalus::values::RuntimeValue> evaluate_boolean_expression (
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
);
std::shared_ptr<daedalus::values::RuntimeValue> evaluate_unary_expression (
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
);
std::shared_ptr<daedalus::values::RuntimeValue> evaluate_binary_expression (
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
);

void setup_interpreter(daedalus::interpreter::Interpreter& interpreter);

#pragma endregion

#pragma region Parser Classes

class BooleanExpression : public daedalus::ast::Expression {
public:
	bool value;

	BooleanExpression(bool value) {
		this->value = value;
	}
	
	virtual std::string type() override {
		return "BooleanExpression";
	}
	virtual std::string repr(int indent = 0) {
		return std::string(indent, '\t') + std::string(this->value ? "true" : "false");
	}
};

class UnaryExpression : public daedalus::ast::Expression {
public:
	UnaryExpression(std::shared_ptr<daedalus::ast::Expression> term, std::string operator_symbol) {
		this->term = term;
		this->operator_symbol = operator_symbol;
	}

	std::shared_ptr<daedalus::ast::Expression> get_term() {
		return this->term;
	}
	std::string get_operator_symbol() {
		return this->operator_symbol;
	}

	virtual std::string type() override {
		return "UnaryExpression";
	}

	virtual std::string repr(int indent = 0) {
		return
			std::string(indent, '\t') + "(\n" +
			std::string(indent + 1, '\t') + this->operator_symbol + "\n" +
			this->term->repr(indent + 1) + "\n" +
			std::string(indent, '\t') + ")";
	}

private:
	std::shared_ptr<daedalus::ast::Expression> term;
	std::string operator_symbol;
};

class BinaryExpression : public daedalus::ast::Expression {
public:
	BinaryExpression(std::shared_ptr<daedalus::ast::Expression> left, std::string operator_symbol, std::shared_ptr<daedalus::ast::Expression> right) {
		this->left = left;
		this->operator_symbol = operator_symbol;
		this->right = right;
	}

	std::shared_ptr<daedalus::ast::Expression> get_left() {
		return this->left;
	}
	std::string get_operator_symbol() {
		return this->operator_symbol;
	}
	std::shared_ptr<daedalus::ast::Expression> get_right() {
		return this->right;
	}
	
	virtual std::string type() override {
		return "BinaryExpression";
	}

	virtual std::string repr(int indent = 0) {
		return
			std::string(indent, '\t') + "(\n" +
			this->left->repr(indent + 1) + "\n" +
			std::string(indent + 1, '\t') + this->operator_symbol + "\n" +
			this->right->repr(indent + 1) + "\n" +
			std::string(indent, '\t') + ")";
	}

private:
	std::shared_ptr<daedalus::ast::Expression> left;
	std::string operator_symbol;
	std::shared_ptr<daedalus::ast::Expression> right;
};

#pragma endregion

#pragma region Interpreter Classes

class BooleanValue: public daedalus::values::RuntimeValue {
public:
	/**
	 * Create a new Null Value
	 */
	BooleanValue(bool value = false) {
		this->value = value;
	}
	
	bool get() {
		return this->value;
	}

	virtual std::string type() override {
		return "BooleanValue";
	}
	
	virtual std::string repr() override {
		return this->value ? "true" : "false";
	}
	
	virtual bool IsTrue() override {
		return this->get();
	}

private:
	bool value;
};

#pragma endregion

int main(int argc, char** argv) {

	daedalus::Daedalus daedalusConfig = daedalus::setup_daedalus(
		setup_lexer,
		setup_parser,
		setup_interpreter
	);

	std::string src = "true && false || !false";
	// std::string src = "3 + 100 * .2 / 1 - 2";
	// std::string src = "(3 + 100 * .2) / (1 - 2)";

	// * LEXER

	std::vector<daedalus::lexer::Token> tokens;

	std::cout << src << std::endl;

	daedalus::lexer::lex(
		daedalusConfig.lexer,
		tokens,
		src
	);

	for(const daedalus::lexer::Token& token : tokens) {
		std::cout << repr(token) << "\n\n";
	}

	// * PARSER

	std::shared_ptr<daedalus::ast::Scope> program = std::make_shared<daedalus::ast::Scope>();

	daedalus::parser::parse(
		daedalusConfig.parser,
		program,
		tokens
	);

	std::cout << program->repr() << std::endl;

	// * INTERPRETER

	std::unordered_map<std::string, std::string> results;

	daedalus::interpreter::interpret(
		daedalusConfig.interpreter,
		results,
		program
	);

	for(const auto& [node, result] : results) {
		std::cout << node << " -> " << result << std::endl;
	}

	return 0;
}

#pragma region Lexer Implementation

void setup_lexer(daedalus::lexer::Lexer& lexer) {
	std::vector<daedalus::lexer::TokenType> tokenTypes = std::vector<daedalus::lexer::TokenType>({
		daedalus::lexer::make_token_type("("),
		daedalus::lexer::make_token_type(")"),
		daedalus::lexer::make_token_type("OPERATOR", "+"),
		daedalus::lexer::make_token_type("OPERATOR", "-"),
		daedalus::lexer::make_token_type("OPERATOR", "*"),
		daedalus::lexer::make_token_type("OPERATOR", "/"),
		daedalus::lexer::make_token_type("OPERATOR", "&&"),
		daedalus::lexer::make_token_type("OPERATOR", "||"),
		daedalus::lexer::make_token_type("UNARY_OPERATOR", "!"),
		daedalus::lexer::make_token_type("BOOL", "true"),
		daedalus::lexer::make_token_type("BOOL", "false"),
		daedalus::lexer::make_token_type(
			"NUMBER",
			[lexer](std::string src) -> std::string {
				std::string number = "";
				size_t i = 0;
				bool isInteger = true;
				while(isdigit(src.at(i)) || src.at(i) == lexer.decimalSeparator) {
					if(src.at(i) == lexer.decimalSeparator) {
						if(!isInteger) {
							throw std::runtime_error("Invalid number format");
						}
						isInteger = false;
					}
					number += src.at(i);
					if(++i == src.length()) {
						break;
					}
				}
				return number;
			}
		)
	});

	daedalus::lexer::setup_lexer(
		lexer,
		tokenTypes
	);
}

#pragma endregion

#pragma region Parser Implementation

std::shared_ptr<daedalus::ast::Expression> parse_boolean_expression(std::vector<daedalus::lexer::Token>& tokens) {
	if(peek(tokens).type == "BOOL") {
		return std::make_shared<BooleanExpression>(eat(tokens).value == "true");
	}
	return daedalus::parser::parse_number_expression(tokens);
}

std::shared_ptr<daedalus::ast::Expression> parse_unary_expression(std::vector<daedalus::lexer::Token>& tokens) {

	std::string operator_symbol = "";

	if(peek(tokens).type == "UNARY_OPERATOR") {
		operator_symbol = eat(tokens).value;
	}
	
	std::shared_ptr<daedalus::ast::Expression> term = parse_boolean_expression(tokens);

	return operator_symbol.size() == 0 ? term : std::make_shared<UnaryExpression>(term, operator_symbol);
}

std::shared_ptr<daedalus::ast::Expression> parse_multiplicative_expression(std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = parse_unary_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "*" || peek(tokens).value == "/")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::ast::Expression> right = parse_multiplicative_expression(tokens);
		
		return std::make_shared<BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<daedalus::ast::Expression> parse_additive_expression(std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = parse_multiplicative_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "+" || peek(tokens).value == "-")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::ast::Expression> right = parse_additive_expression(tokens);
		
		return std::make_shared<BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<daedalus::ast::Expression> parse_logical_expression(std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = parse_additive_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "&&" || peek(tokens).value == "||")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::ast::Expression> right = parse_logical_expression(tokens);
		
		return std::make_shared<BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<daedalus::ast::Expression> parse_binary_expression(std::vector<daedalus::lexer::Token>& tokens) {
	return parse_logical_expression(tokens);
}

void setup_parser(daedalus::parser::Parser& parser) {

	daedalus::parser::setup_parser(
		parser,
		{
			{
				"BooleanExpression",
				daedalus::parser::make_node(&parse_boolean_expression, false)
			},
			{
				"UnaryExpression",
				daedalus::parser::make_node(&parse_unary_expression, false)
			},
			{
				"BinaryExpression",
				daedalus::parser::make_node(&parse_binary_expression)
			},
		}
	);

	daedalus::parser::demoteTopNode(parser, "NumberExpression");
}

#pragma endregion

#pragma region Interpreter Implementation

bool validate_mutability(daedalus::env::EnvValue env_value, std::shared_ptr<daedalus::values::RuntimeValue> new_value) {
	try {
		return env_value.properties.at("isMutable") == "true";
	} catch(const std::exception& e) {
		throw std::runtime_error("Trying to access undeclared property \"isMutable\"");
	}
}

std::string get_mutability_error_message(std::string key, std::string on) {
	return "Trying to set immutable value \"" + key + "\"";
}

std::shared_ptr<daedalus::values::RuntimeValue> evaluate_boolean_expression (
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<BooleanExpression> booleanExpression = std::dynamic_pointer_cast<BooleanExpression>(statement);

	return std::make_shared<BooleanValue>(booleanExpression->value);
};

std::shared_ptr<daedalus::values::RuntimeValue> evaluate_unary_expression (
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<UnaryExpression> unaryExpression = std::dynamic_pointer_cast<UnaryExpression>(statement);

	std::shared_ptr<daedalus::values::RuntimeValue> term = daedalus::interpreter::evaluate_statement(interpreter, unaryExpression->get_term(), env);
	std::string operator_symbol = unaryExpression->get_operator_symbol();

	if(operator_symbol == "!") {
		return std::make_shared<BooleanValue>(!term->IsTrue());
	}

	throw std::runtime_error("Unknown unary operator " + operator_symbol);
}

std::shared_ptr<daedalus::values::RuntimeValue> evaluate_binary_expression (
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<BinaryExpression> binaryExpression = std::dynamic_pointer_cast<BinaryExpression>(statement);

	std::shared_ptr<daedalus::values::RuntimeValue> left = daedalus::interpreter::evaluate_statement(interpreter, binaryExpression->get_left(), env);
	std::shared_ptr<daedalus::values::RuntimeValue> right = daedalus::interpreter::evaluate_statement(interpreter, binaryExpression->get_right(), env);

	std::string operator_symbol = binaryExpression->get_operator_symbol();

	if(operator_symbol == "+") {
		if(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue"
		) {
			throw std::runtime_error("Trying to add invalid operands");
		}

		double left_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(right)->get();
		
		return std::make_shared<daedalus::values::NumberValue>(left_nb + right_nb);
	}
	if(operator_symbol == "-") {
		if(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue"
		) {
			throw std::runtime_error("Trying to subtract invalid operands");
		}
		
		double left_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(right)->get();
		
		return std::make_shared<daedalus::values::NumberValue>(left_nb - right_nb);
	}
	if(operator_symbol == "*") {
		if(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue"
		) {
			throw std::runtime_error("Trying to multiply invalid operands");
		}
		
		double left_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(right)->get();
		
		return std::make_shared<daedalus::values::NumberValue>(left_nb * right_nb);
	}
	if(operator_symbol == "/") {
		if(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue"
		) {
			throw std::runtime_error("Trying to divide invalid operands");
		}
		
		double left_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(right)->get();

		if(right_nb == 0) {
			throw std::runtime_error("Trying to divide by zero");
		}
		
		return std::make_shared<daedalus::values::NumberValue>(left_nb / right_nb);
	}
	if(operator_symbol == "&&") {
		return std::make_shared<BooleanValue>(left->IsTrue() && right->IsTrue());
	}
	if(operator_symbol == "||") {
		return std::make_shared<BooleanValue>(left->IsTrue() || right->IsTrue());
	}

	throw std::runtime_error("Unknown operator " + operator_symbol);
}

void setup_interpreter(daedalus::interpreter::Interpreter& interpreter) {

	std::unordered_map<std::string, daedalus::interpreter::ParseStatementFunction> nodeEvaluationFunctions = std::unordered_map<std::string, daedalus::interpreter::ParseStatementFunction>({
		{ "BooleanExpression", &evaluate_boolean_expression },
		{ "UnaryExpression", &evaluate_unary_expression },
		{ "BinaryExpression", &evaluate_binary_expression },
	});

	std::vector<std::string> envValuesProperties = std::vector<std::string>({
		"isMutable"
	});

	daedalus::env::EnvValidationRule mutabilityValidation = {
		&get_mutability_error_message,
		&validate_mutability,
		std::vector<daedalus::env::ValidationRuleSensitivity>({
			daedalus::env::ValidationRuleSensitivity::SET
		})
	};

	std::vector<daedalus::env::EnvValidationRule> validationRules = std::vector<daedalus::env::EnvValidationRule>({
		mutabilityValidation
	});

	daedalus::interpreter::setup_interpreter(
		interpreter,
		nodeEvaluationFunctions,
		envValuesProperties,
		validationRules
	);

}

#pragma endregion