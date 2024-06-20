#include <AquIce/daedalus/parser.hpp>

[[nodiscard]] daedalus::lexer::Token peek(std::vector<daedalus::lexer::Token>& tokens) {
	return tokens.front();
}

[[nodiscard]] daedalus::lexer::Token eat(std::vector<daedalus::lexer::Token>& tokens) {
	daedalus::lexer::Token first = peek(tokens);
	tokens.erase(tokens.begin());
	return first;
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
	if(peek(tokens).type == "NUMBER") {
		return std::make_shared<daedalus::ast::NumberExpression>(std::stod(eat(tokens).value));
	}
	return nullptr;
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
	std::unordered_map<std::string, Node> nodesRegister
) {
	parser.nodesRegister = nodesRegister;
	daedalus::parser::register_node(
		parser,
		"NumberExpression",
		daedalus::parser::make_node(&parse_number_expression)
	);
}

std::shared_ptr<daedalus::ast::Statement> daedalus::parser::parse_statement(
	daedalus::parser::Parser& parser,
	std::vector<daedalus::lexer::Token>& tokens
) {
	std::shared_ptr<daedalus::ast::Statement> statement = nullptr;

	for(auto& [key, node] : parser.nodesRegister) {
		if(node.isTopNode && statement == nullptr) {
			statement = node.parse_node(tokens);
		}
	}

	if(statement == nullptr) {
		throw std::runtime_error("Unknown token : \"" + peek(tokens).type + "\"");
	}

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