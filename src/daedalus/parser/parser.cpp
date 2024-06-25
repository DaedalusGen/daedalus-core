#include <AquIce/daedalus/parser.hpp>

[[nodiscard]] daedalus::lexer::Token peek(std::vector<daedalus::lexer::Token>& tokens) {
	return tokens.front();
}

[[nodiscard]] daedalus::lexer::Token eat(std::vector<daedalus::lexer::Token>& tokens) {
	daedalus::lexer::Token first = peek(tokens);
	tokens.erase(tokens.begin());
	return first;
}

[[nodiscard]] daedalus::lexer::Token expect(std::vector<daedalus::lexer::Token>& tokens, std::string tokenType, std::string error) {
	daedalus::lexer::Token token = eat(tokens);
	
	DAE_ASSERT_TRUE(
		token.type == tokenType,
		error
	)

	return token;
}

daedalus::parser::Node daedalus::parser::make_node(
	daedalus::parser::ParseNodeFunction parse_node,
	bool isTopNode
) {
	return daedalus::parser::Node{
		parse_node,
		isTopNode
	};
}

void daedalus::parser::demoteTopNode(
	daedalus::parser::Parser& parser,
	std::string key
) {
	parser.nodesRegister.at(key).isTopNode = false;
}

std::shared_ptr<daedalus::ast::Expression> daedalus::parser::parse_number_expression(std::vector<daedalus::lexer::Token>& tokens) {
	return std::make_shared<daedalus::ast::NumberExpression>(std::stod(expect(
		tokens,
		"NUMBER",
		"Unknown token found (type: " + peek(tokens).type + ", value: " + peek(tokens).value + ")"
	).value));
}

void daedalus::parser::register_node(
	daedalus::parser::Parser& parser,
	std::string key,
	daedalus::parser::Node node
) {
	parser.nodesRegister[key] = node;
}

void daedalus::parser::setup_parser(
	Parser& parser,
	std::unordered_map<std::string, Node> nodesRegister,
	std::vector<daedalus::parser::ParserFlags> flags
) {
	parser.nodesRegister = nodesRegister;
	daedalus::parser::register_node(
		parser,
		"NumberExpression",
		daedalus::parser::make_node(&parse_number_expression)
	);
	parser.flags = flags;
}

bool daedalus::parser::has_flag(
	daedalus::parser::Parser& parser,
	daedalus::parser::ParserFlags flag
) {
	return std::find(
		parser.flags.begin(),
		parser.flags.end(),
		flag
	) != parser.flags.end();
}

std::shared_ptr<daedalus::ast::Statement> daedalus::parser::parse_statement(
	daedalus::parser::Parser& parser,
	std::vector<daedalus::lexer::Token>& tokens
) {
	std::shared_ptr<daedalus::ast::Statement> statement = nullptr;

	for(auto& [key, node] : parser.nodesRegister) {
		if(node.isTopNode && statement == nullptr) {
			statement = node.parse_node(tokens);
			if(!has_flag(parser, daedalus::parser::ParserFlags::OPTI_CONST_EXPR)) {
				continue;
			}
			if(std::shared_ptr<daedalus::ast::Expression> expression = std::dynamic_pointer_cast<daedalus::ast::Expression>(statement)) {
				statement = expression->get_constexpr();
			}
		}
	}

	DAE_ASSERT_TRUE(
		statement != nullptr,
		std::runtime_error("Unknown token found (type: " + peek(tokens).type + ", value: " + peek(tokens).value + ")")
	)

	return statement;
}

void daedalus::parser::parse(
	daedalus::parser::Parser& parser,
	std::shared_ptr<daedalus::ast::Scope> program,
	std::vector<daedalus::lexer::Token>& tokens
) {
	while(peek(tokens).type != "EOF") {
		program->body.push_back(
			parse_statement(parser, tokens)
		);
	}
}