#include <daedalus/core/parser/parser.hpp>

[[nodiscard]] daedalus::core::lexer::Token peek(std::vector<daedalus::core::lexer::Token>& tokens) {
	return tokens.front();
}

[[nodiscard]] daedalus::core::lexer::Token eat(std::vector<daedalus::core::lexer::Token>& tokens) {
	daedalus::core::lexer::Token first = peek(tokens);
	tokens.erase(tokens.begin());
	return first;
}

[[nodiscard]] daedalus::core::lexer::Token expect(std::vector<daedalus::core::lexer::Token>& tokens, std::string tokenType, std::runtime_error error) {
	daedalus::core::lexer::Token token = eat(tokens);

	DAE_ASSERT_TRUE(
		token.type == tokenType,
		error
	)

	return token;
}

daedalus::core::parser::Node daedalus::core::parser::make_node(
	daedalus::core::parser::ParseNodeFunction parse_node,
	bool isTopNode
) {
	return daedalus::core::parser::Node{
		parse_node,
		isTopNode
	};
}

void daedalus::core::parser::demoteTopNode(
	daedalus::core::parser::Parser& parser,
	std::string key
) {
	parser.nodesRegister.at(key).isTopNode = false;
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::core::parser::parse_number_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	return std::make_shared<daedalus::core::ast::NumberExpression>(std::stod(expect(
		tokens,
		"NUMBER",
		std::runtime_error("Unknown token found (type: " + peek(tokens).type + ", value: " + peek(tokens).value + ")")
	).value));
}

void daedalus::core::parser::register_node(
	daedalus::core::parser::Parser& parser,
	std::string key,
	daedalus::core::parser::Node node
) {
	parser.nodesRegister[key] = node;
}

void daedalus::core::parser::setup_parser(
	Parser& parser,
	std::unordered_map<std::string, Node> nodesRegister,
	std::vector<daedalus::core::parser::ParserFlags> flags
) {
	parser.nodesRegister = nodesRegister;
	daedalus::core::parser::register_node(
		parser,
		"NumberExpression",
		daedalus::core::parser::make_node(&parse_number_expression)
	);
	parser.flags = flags;
}

bool daedalus::core::parser::has_flag(
	daedalus::core::parser::Parser& parser,
	daedalus::core::parser::ParserFlags flag
) {
	return std::find(
		parser.flags.begin(),
		parser.flags.end(),
		flag
	) != parser.flags.end();
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::core::parser::parse_expression(
	daedalus::core::parser::Parser& parser,
	std::vector<daedalus::core::lexer::Token>& tokens,
	bool needsSemicolon
) {
	std::shared_ptr<daedalus::core::ast::Expression> expression = nullptr;

	for(auto& [key, node] : parser.nodesRegister) {
		if(node.isTopNode) {
			expression = node.parse_node(parser, tokens, needsSemicolon);
			if(!has_flag(parser, daedalus::core::parser::ParserFlags::OPTI_CONST_EXPR)) {
				return expression;
			}
			return expression->get_constexpr();
		}
	}

	throw std::runtime_error("Unknown token found (type: " + peek(tokens).type + ", value: " + peek(tokens).value + ")");
}

void daedalus::core::parser::parse(
	daedalus::core::parser::Parser& parser,
	std::shared_ptr<daedalus::core::ast::Scope> program,
	std::vector<daedalus::core::lexer::Token>& tokens
) {
	while(peek(tokens).type != "EOF") {
		program->push_back_body(
			parse_expression(parser, tokens, true)
		);
	}
}
