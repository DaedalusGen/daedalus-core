# Summary

- [Summary](#summary)
- [Setup](#setup)
	- [Entry point](#entry-point)
	- [Lexer](#lexer)
		- [Configuration](#configuration)
			- [Custom Token Types](#custom-token-types)
			- [More Settings](#more-settings)
		- [Execution](#execution)
		- [Result](#result)
		- [Code](#code)
	- [Parser](#parser)
		- [Configuration](#configuration-1)
			- [Custom Node Classes](#custom-node-classes)
			- [Custom Parsing Functions](#custom-parsing-functions)
			- [Actual Configuration](#actual-configuration)
		- [Execution](#execution-1)
		- [Result](#result-1)
		- [Code](#code-1)
	- [Interpreter](#interpreter)
		- [Configuration](#configuration-2)
			- [Custom Node Classes](#custom-node-classes-1)
			- [Custom Evaluation Functions](#custom-evaluation-functions)
			- [Custom Environment Validation](#custom-environment-validation)
				- [Custom Environment Properties](#custom-environment-properties)
					- [Custom Properties Validation](#custom-properties-validation)
			- [Actual Configuration](#actual-configuration-1)
		- [Execution](#execution-2)
		- [Result](#result-2)
		- [Code](#code-2)
	- [Core Setup](#core-setup)

# Setup

## Entry point

To start, you need a basic C++ program which contains a source string :

```cpp
// main.cpp

int main(void) {
	std::string src = "3 + 100 * .2 / 1 - 2";
	return 0;
}
```

## Lexer

### Configuration

To run your custom lexer, you first need to import the header file.

```cpp
// main.cpp

#include <AquIce/daedalus/lexer.hpp>
```

Then, instanciate the `Lexer` struct, found in `daedalus::lexer`.

```cpp
// main.cpp > main

daedalus::lexer::Lexer lexer;
```

After that, you have to load a custom configuration into this lexer.

For the sake of code readability, we will move this code in its own function : `setup_lexer`.

Here is this function's header :

```cpp
// main.cpp

void setup_lexer(daedalus::lexer::Lexer& lexer);
```

#### Custom Token Types

Inside this function, we will first create a list of all valid token types.

We can create token types 3 different ways:

1. A Token Type whose value is the same as its type.
   
	For example, an open parenthesis :
	
	```cpp
	daedalus::lexer::make_token_type("(")
	```

2. A Token Type who has a value and a name.

	For example, a `+` operator :

	```cpp
	daedalus::lexer::make_token_type("OPERATOR", "+")
	```

3. A Token type who has a name, and a custom value lexing function.

	For example, a number :

	```cpp
	daedalus::lexer::make_token_type(
		"NUMBER",
		&lex_number
	)
	```

	> Note: `lex_number` is a function that lexes the value of a number.
	>
	> It can also be input as a lamba function

Finally, we put all those tokens in a list we can use later.

```cpp
// main.cpp > setup_lexer

std::vector<daedalus::lexer::TokenType> tokenTypes = std::vector<daedalus::lexer::TokenType>({
	daedalus::lexer::make_token_type("("),
	daedalus::lexer::make_token_type(")"),
	daedalus::lexer::make_token_type("OPERATOR", "+"),
	daedalus::lexer::make_token_type("OPERATOR", "-"),
	daedalus::lexer::make_token_type("OPERATOR", "*"),
	daedalus::lexer::make_token_type("OPERATOR", "/"),
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
```

> Note: This implementation example uses a lamba function as the lexing function for the `NUMBER` Token Type

Then, we can call the `setup_lexer` function, found in `daedalus::lexer`, to configure our tokens.

```cpp
// main.cpp > setup_lexer

daedalus::lexer::setup_lexer(
	lexer,
	tokenTypes
);
```

#### More Settings

If you want to go further, you can customize the following things :

- Whitespaces
- Single line comments
- Multi line comments

```cpp
// main.cpp > setup_lexer

daedalus::lexer::setup_lexer(
	lexer,
	tokenTypes,
	std::vector<char>({
		' ',
		'\t',
		'\n',
		'\r',
	}),
	"//",
	{ "/*", "*/" }
);
```

> Note: The values shown above are the default values.

### Execution

To execute our lexer, we first need to call the function we just created.

```cpp
// main.cpp > main

setup_lexer(lexer);
```

Then, we create an empty token list, and call the `lex` function from `daedalus::lexer` to lex the source string.

```cpp
// main.cpp > main

std::vector<daedalus::lexer::Token> tokens;

daedalus::lexer::lex(
	lexer,
	tokens,
	src
);
```

### Result

We can then iterate through the same list to display the result of the lexing.

```cpp
for(const daedalus::lexer::Token& token : tokens) {
	std::cout << repr(token) << "\n\n";
}
```

### Code

Here is the entire file containing the full lexer configuration :

```cpp
// main.cpp

#include <iostream>
#include <string>

#include <AquIce/daedalus/lexer.hpp>

void setup_lexer(daedalus::lexer::Lexer& lexer);

int main(int argc, char** argv) {

	std::string src = "3 + 100 * .2 / 1 - 2";
	// std::string src = "(3 + 100 * .2) / (1 - 2)";

	// * LEXER

	daedalus::lexer::Lexer lexer;

	setup_lexer(lexer);

	std::vector<daedalus::lexer::Token> tokens;

	std::cout << src << std::endl;

	daedalus::lexer::lex(
		lexer,
		tokens,
		src
	);

	for(const daedalus::lexer::Token& token : tokens) {
		std::cout << repr(token) << "\n\n";
	}

	return 0;
}

void setup_lexer(daedalus::lexer::Lexer& lexer) {
	std::vector<daedalus::lexer::TokenType> tokenTypes = std::vector<daedalus::lexer::TokenType>({
		daedalus::lexer::make_token_type("OPERATOR", "+"),
		daedalus::lexer::make_token_type("OPERATOR", "-"),
		daedalus::lexer::make_token_type("OPERATOR", "*"),
		daedalus::lexer::make_token_type("OPERATOR", "/"),
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
```

## Parser

### Configuration

To run your custom parser, you first need to import the header file.

```cpp
// main.cpp

#include <AquIce/daedalus/parser.hpp>
```

Then, instanciate the `Parser` struct, found in `daedalus::parser`.

```cpp
// main.cpp > main

daedalus::parser::Parser parser;
```

After that, you have to load a custom configuration into this parser.

As for the lexer, we will move this code in its own function : `setup_parser`.

Here is this function's header :

```cpp
// main.cpp

void setup_parser(daedalus::parser::Parser& parser);
```

#### Custom Node Classes

To add a new Node to the AST, we need to create a class representing it.

There are 4 built-in Nodes :

1. `Statement`
	
	The base class, only used for inheritance.

2. `Scope`

	Inherits from `Statement` and is used to store the program.

3. `Expression`

	Inherits from `Statement`, only used for inheritance.

4. `NumberExpression`

	Inherits from `Expression`, holds a single number value.

In order to create a custom Node class, you need to do a few things:

1. Your class must inherit from `Statement` or one of its derived classes

	```cpp
	// main.cpp

	class BinaryExpression : public daedalus::ast::Expression {}
	```

2. You need to implement the virtual method `type`

	```cpp
	// main.cpp > BinaryExpression

	virtual std::string type() override {
		return "BinaryExpression";
	}
	```

3. And the `repr` method as well

	```cpp
	// main.cpp > BinaryExpression

	virtual std::string repr(int indent = 0) {
		return
			std::string(indent, '\t') + "(\n" +
			this->left->repr(indent + 1) + "\n" +
			std::string(indent + 1, '\t') + this->operator_symbol + "\n" +
			this->right->repr(indent + 1) + "\n" +
			std::string(indent, '\t') + ")";
	```

Then, you can add any data your class might need, as well as a custom constructor :

```cpp
// main.cpp > BinaryExpression

public:
	BinaryExpression(
		std::shared_ptr<daedalus::ast::Expression> left,
		std::string operator_symbol,
		std::shared_ptr<daedalus::ast::Expression> right
	) {
		this->left = left;
		this->operator_symbol = operator_symbol;
		this->right = right;
	}
```
```cpp
// main.cpp > BinaryExpression

private:
	std::shared_ptr<daedalus::ast::Expression> left;
	std::string operator_symbol;
	std::shared_ptr<daedalus::ast::Expression> right;
```

#### Custom Parsing Functions

Now that we have the Node, we need a way to parse it.

We have to declare a function that takes a reference to a vector of tokens as argument and returns a shared pointer to an Expression.

As a convention, we will name the functions `parse_<thing_to_parse>`

```cpp
std::shared_ptr<daedalus::ast::Expression> parse_any(std::vector<daedalus::lexer::Token>& tokens);
```

To simplify the implementation, as well as add the concept of precedence, we will split the implementation between three methods: 

- `parse_multiplicative_expression`
- `parse_additive_expression`
- `parse_binary_expression`

```cpp
std::shared_ptr<daedalus::ast::Expression> parse_multiplicative_expression(std::vector<daedalus::lexer::Token>& tokens);
std::shared_ptr<daedalus::ast::Expression> parse_additive_expression(std::vector<daedalus::lexer::Token>& tokens);
std::shared_ptr<daedalus::ast::Expression> parse_binary_expression(std::vector<daedalus::lexer::Token>& tokens);
```

On the other hand, to keep this explanation concise, we will only look at the implementation of `parse_multiplicative_expression`, as the other functions are basically the same.

First, we need to parse the left of the expression as a Number Expression.

```cpp
// main.cpp > parse_multiplicative_expression

std::shared_ptr<daedalus::ast::Expression> left = daedalus::parser::parse_number_expression(tokens);
```

Then, if the left is `nullptr`, we return it, as it means we encountered an unknown token.

```cpp
// main.cpp > parse_multiplicative_expression

if(left == nullptr) {
	return nullptr;
}
```

Then, we need to check if the next token is a valid multiplication operator.

```cpp
// main.cpp > parse_multiplicative_expression

if(
	peek(tokens).type == "OPERATOR" &&
	(peek(tokens).value == "*" || peek(tokens).value == "/")
)
```

- If the token was valid, we can eat it, and parse the right as a Binary Expression

	We can then return a new Binary Expression containing the left, the operator and the right.

	```cpp
	// main.cpp > parse_multiplicative_expression

	std::string operator_symbol = eat(tokens).value;
	std::shared_ptr<daedalus::ast::Expression> right = parse_multiplicative_expression(tokens);
	return std::make_shared<BinaryExpression>(
		left,
		operator_symbol,
		right
	);
	```

- If the token is not valid, we return the left of the Expression

	```cpp
	// main.cpp > parse_multiplicative_expression

	return left;
	```

#### Actual Configuration

Now that we have everything, we just need to link it all together in our `setup_parser` function.

First, we call the `setup_parser` function found in `daedalus::parser`.

We pass the following as arguments :
- The parser
- The Node Types we want to add as parameters (in our example only `BinaryExpression`)
- The list of the parser flags (defaults to `{ OPTI_CONST_EXPR }`)

```cpp
daedalus::parser::setup_parser(
	parser,
	{
		{
			"BinaryExpression",
			daedalus::parser::make_node(&parse_binary_expression)
		}
	},,
	std::vector<daedalus::parser::ParserFlags>()
);
```

A really important notion to understand is whether a Node Type is a `topNode` or not.

Being a top Node means that this Node has to be parsed by `daedalus::parser::parse_statement`.

A non-top Node is a Node that is parsed by another (for example `NumberExpression`).

We use the `demoteTopNode` function to make `NumberExpression` a non-top Node.

```cpp
daedalus::parser::demoteTopNode(parser, "NumberExpression");
```

### Execution

To execute our parser, we first need to call the function we just created.

```cpp
// main.cpp > main

setup_parser(parser);
```

Then, we create a Program Node, and call the `parse` function from `daedalus::parser` to parse the token list.

```cpp
// main.cpp > main

std::shared_ptr<daedalus::ast::Scope> program = std::make_shared<daedalus::ast::Scope>();

daedalus::parser::parse(
	parser,
	program,
	tokens
);
```

### Result

We can display the result of the parsing with 

```cpp
std::cout << program->repr() << std::endl;
```

### Code

Here is the entire file containing the full parser and lexer configuration :

```cpp
// main.cpp > main

#include <iostream>
#include <string>

#include <AquIce/daedalus/lexer.hpp>
#include <AquIce/daedalus/parser.hpp>

#pragma region Prototypes

void setup_lexer(daedalus::lexer::Lexer& lexer);

std::shared_ptr<daedalus::ast::Expression> parse_multiplicative_expression(std::vector<daedalus::lexer::Token>& tokens);
std::shared_ptr<daedalus::ast::Expression> parse_additive_expression(std::vector<daedalus::lexer::Token>& tokens);
std::shared_ptr<daedalus::ast::Expression> parse_binary_expression(std::vector<daedalus::lexer::Token>& tokens);

void setup_parser(daedalus::parser::Parser& parser);

#pragma endregion

#pragma region Classes

class BinaryExpression : public daedalus::ast::Expression {
public:
	BinaryExpression(std::shared_ptr<daedalus::ast::Expression> left, std::string operator_symbol, std::shared_ptr<daedalus::ast::Expression> right) {
		this->left = left;
		this->operator_symbol = operator_symbol;
		this->right = right;
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

int main(int argc, char** argv) {

	std::string src = "3 + 100 * .2 / 1 - 2";

	// * LEXER

	daedalus::lexer::Lexer lexer;

	setup_lexer(lexer);

	std::vector<daedalus::lexer::Token> tokens;

	std::cout << src << std::endl;

	daedalus::lexer::lex(
		lexer,
		tokens,
		src
	);

	for(const daedalus::lexer::Token& token : tokens) {
		std::cout << repr(token) << "\n\n";
	}

	// * PARSER

	daedalus::parser::Parser parser;

	setup_parser(parser);

	std::shared_ptr<daedalus::ast::Scope> program = std::make_shared<daedalus::ast::Scope>();

	daedalus::parser::parse(
		parser,
		program,
		tokens
	);

	std::cout << program->repr() << std::endl;

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

std::shared_ptr<daedalus::ast::Expression> parse_multiplicative_expression(std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = daedalus::parser::parse_number_expression(tokens);

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

std::shared_ptr<daedalus::ast::Expression> parse_binary_expression(std::vector<daedalus::lexer::Token>& tokens) {
	return parse_additive_expression(tokens);
}

void setup_parser(daedalus::parser::Parser& parser) {

	daedalus::parser::setup_parser(
		parser,
		{
			{
				"BinaryExpression",
				daedalus::parser::make_node(&parse_binary_expression)
			}
		}
	);

	daedalus::parser::demoteTopNode(parser, "NumberExpression");
}

#pragma endregion
```

## Interpreter

### Configuration

To run your custom interpreter, you first need to import the header file.

```cpp
// main.cpp

#include <AquIce/daedalus/interpreter.hpp>
```

Then, instanciate the `Interpreter` struct, found in `daedalus::interpreter`.

```cpp
// main.cpp > main

daedalus::interpreter::Interpreter interpreter;
```

After that, you have to load a custom configuration into this interpreter.

As for both the lexer and parser, we will move this code in its own function : `setup_interpreter`.

Here is this function's header :

```cpp
// main.cpp

void setup_interpreter(daedalus::interpreter::Interpreter& interpreter);
```

#### Custom Node Classes

To add a new Value Type, we need to create a class representing it.

There are 3 built-in Value Types :

1. `RuntimeValue`
	
	The base class, only used for inheritance.

2. `NullValue`

	Inherits from `RuntimeValue` and holds no value.

3. `NumberValue`

	Inherits from `RuntimeValue`, holds the value of a number.

In order to create a custom Node class, you need to do a few things:

1. Your class must inherit from `RuntimeValue` or one of its derived classes

	```cpp
	// main.cpp

	class BooleanValue: public daedalus::values::RuntimeValue {}
	```

2. You need to implement the virtual method `type`

	```cpp
	// main.cpp > BooleanValue

	virtual std::string type() override {
		return "BooleanValue";
	}
	```

3. And the `repr` method as well

	```cpp
	// main.cpp > BooleanValue

	virtual std::string repr() override {
		return this->value ? "true" : "false";
	}
	```

4. And finally the `isTrue` method

	```cpp
	// main.cpp > BooleanValue

	virtual bool IsTrue() override {
		return this->get();
	}
	```

Then, you can add any data your class might need, as well as a custom constructor :

```cpp
// main.cpp > BooleanValue

public:
	BooleanValue(bool value = false) {
		this->value = value;
	}
	
	bool get() {
		return this->value;
	}
```
```cpp
// main.cpp > BooleanValue

private:
	bool value;
```

#### Custom Evaluation Functions

Now that we need a way to evaluate our AST Nodes to get output.

We have to declare a function that takes in:

- A reference to an interpreter
- A shared pointer to a Statement (or derived class)
- A shared pointer to an Environment

And returns a shared pointer to a RuntimeValue.

As a convention, we will name the functions `evaluate_<thing_to_evaluate>`

```cpp
std::shared_ptr<daedalus::values::RuntimeValue> evaluate_any (
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
);
```

#### Custom Environment Validation

##### Custom Environment Properties

We can also add custom properties to values that will be stored in an environment (needs `Declaration`).

To do that, we only need to specify the name of the property (all property values are stored as string).

We can for example add an `isMutable` property.

```cpp
// main.cpp > setup_interpreter

std::vector<std::string> envValuesProperties = std::vector<std::string>({
	"isMutable"
});
```

###### Custom Properties Validation

Then, we need to use those properties.

We can define the validation rule, and when it should be used (on variable `init`, `set`, or `get`).

A validation function is named following the pattern `validate_<thing_to_validate>`.

It takes in :

- The destination value as an `EnvValue`
- The new value as a shared pointer to a `RuntimeValue`
- The key of the value to set

and outputs the resulting EnvValue or throws if the rule is violated.

```cpp
daedalus::env::EnvValue validate_any(daedalus::env::EnvValue env_value, std::shared_ptr<daedalus::values::RuntimeValue> new_value, std::string key);
```

The function to validate the mutability should look something like this :

```cpp
// main.cpp

daedalus::env::EnvValue validate_mutability(daedalus::env::EnvValue env_value, std::shared_ptr<daedalus::values::RuntimeValue> new_value, std::string key) {
	try {
		if(env_value.properties.at("isMutable") == "true") {
			return daedalus::env::EnvValue{ new_value, env_value.properties };
		}
	} catch(const std::exception& e) {
		throw std::runtime_error("Trying to access undeclared property \"isMutable\"");
	}
	throw std::runtime_error("Trying to assign to immutable value \"" + key + "\"");
}
```

#### Actual Configuration

Now that we have everything, we just need to link it all together in our `setup_interpreter` function.

First, we call the `setup_interpreter` function found in `daedalus::interpreter`.

We need to pass 3 configuration arguments :

- The evaluation functions for each Node Type we created (an unordered map of string to `ParseStatementFunction`)
- The properties to add to environment values (a vector of strings)
- The validation rules to add to these properties (a vector of `EnvValidationRule`)

We pass the Node Types we want to add as parameters (in our example only `BinaryExpression`).

```cpp
std::unordered_map<std::string, daedalus::interpreter::ParseStatementFunction> nodeEvaluationFunctions = std::unordered_map<std::string, daedalus::interpreter::ParseStatementFunction>({
	{ "BooleanExpression", &evaluate_boolean_expression },
	{ "UnaryExpression", &evaluate_unary_expression },
	{ "BinaryExpression", &evaluate_binary_expression },
});

std::vector<std::string> envValuesProperties = std::vector<std::string>({
	"isMutable"
});

daedalus::env::EnvValidationRule mutabilityValidation = {
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
```

### Execution

To execute our parser, we first need to call the function we just created.

```cpp
// main.cpp > main

setup_interpreter(interpreter);
```

Then, we create a map of results, and call the `interpret` function from `daedalus::interpreter` to interpret the Node list.

```cpp
// main.cpp > main

std::unordered_map<std::string, std::string> results;

daedalus::interpreter::interpret(
	interpreter,
	results,
	program
);
```

### Result

We can display the result of the program with 

```cpp
for(const auto& [node, result] : results) {
	std::cout << node << " -> " << result << std::endl;
}
```

### Code

Here is the entire file containing the full interpreter, parser and lexer configuration :

```cpp
// main.cpp

#include <iostream>
#include <string>
#include <vector>

#include <AquIce/daedalus/lexer.hpp>
#include <AquIce/daedalus/parser.hpp>
#include <AquIce/daedalus/interpreter.hpp>

#pragma region Prototypes

void setup_lexer(daedalus::lexer::Lexer& lexer);

std::shared_ptr<daedalus::ast::Expression> parse_multiplicative_expression(std::vector<daedalus::lexer::Token>& tokens);
std::shared_ptr<daedalus::ast::Expression> parse_additive_expression(std::vector<daedalus::lexer::Token>& tokens);
std::shared_ptr<daedalus::ast::Expression> parse_binary_expression(std::vector<daedalus::lexer::Token>& tokens);

void setup_parser(daedalus::parser::Parser& parser);

bool validate_mutability(daedalus::env::EnvValue value);

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

	std::string src = "true && false || !false";
	// std::string src = "3 + 100 * .2 / 1 - 2";
	// std::string src = "(3 + 100 * .2) / (1 - 2)";

	// * LEXER

	daedalus::lexer::Lexer lexer;

	setup_lexer(lexer);

	std::vector<daedalus::lexer::Token> tokens;

	std::cout << src << std::endl;

	daedalus::lexer::lex(
		lexer,
		tokens,
		src
	);

	for(const daedalus::lexer::Token& token : tokens) {
		std::cout << repr(token) << "\n\n";
	}

	// * PARSER

	daedalus::parser::Parser parser;

	setup_parser(parser);

	std::shared_ptr<daedalus::ast::Scope> program = std::make_shared<daedalus::ast::Scope>();

	daedalus::parser::parse(
		parser,
		program,
		tokens
	);

	std::cout << program->repr() << std::endl;

	// * INTERPRETER

	daedalus::interpreter::Interpreter interpreter;

	setup_interpreter(interpreter);

	std::unordered_map<std::string, std::string> results;

	daedalus::interpreter::interpret(
		interpreter,
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

bool validate_mutability(daedalus::env::EnvValue value) {
	try {
		return value.properties.at("isMutable") == "true";
	} catch(const std::exception& e) {
		throw std::runtime_error("Trying to access undeclared property \"isMutable\"");
	}
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
```

## Core Setup

You can also setup all of that faster, by using the function `setup_daedalus` from `daedalus`.

To do that, you need to pass three arguments :

- The `setup_lexer` function

	```cpp
	void setup_lexer(daedalus::lexer::Lexer& lexer);
	```

- The `setup_parser` function

	```cpp
	void setup_parser(daedalus::parser::Parser& parser);
	```

- The `setup_interpreter` function

	```cpp
	void setup_interpreter(daedalus::interpreter::Interpreter& interpreter);
	```

```cpp
// main.cpp > main

daedalus::Daedalus daedalusConfig = daedalus::setup_daedalus(
	&setup_lexer,
	&setup_parser,
	&setup_interpreter
);
```

Now, when you need to call `lex`, `parse` or `interpret`, you can pass `daedalusConfig.<lexer/parser/interpreter>` as the lexer / parser / interpreter.