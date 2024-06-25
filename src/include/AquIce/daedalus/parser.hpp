#ifndef __DAEDALUS_PARSER__
#define __DAEDALUS_PARSER__

#include "lexer.hpp"
#include "ast.hpp"
#include "unit.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#define generate_parser(size)

[[nodiscard]] daedalus::lexer::Token peek(std::vector<daedalus::lexer::Token>& tokens);

[[nodiscard]] daedalus::lexer::Token eat(std::vector<daedalus::lexer::Token>& tokens);

namespace daedalus {
	namespace parser {

		typedef std::function<std::shared_ptr<daedalus::ast::Statement> (std::vector<daedalus::lexer::Token>&)> ParseNodeFunction;

		typedef struct Node {
			ParseNodeFunction parse_node;
			bool isTopNode;
		} Node;

		enum class ParserFlags {
			OPTI_CONST_EXPR,
		};

		typedef struct Parser {
			std::unordered_map<std::string, Node> nodesRegister;
			std::vector<ParserFlags> flags;
		} Parser;

		Node make_node(
			ParseNodeFunction parse_node,
			bool isTopNode = true
		);

		void demoteTopNode(
			Parser& parser,
			std::string key
		);

		std::shared_ptr<daedalus::ast::Expression> parse_number_expression(std::vector<daedalus::lexer::Token>& tokens);

		void register_node(
			Parser& parser,
			std::string key,
			Node node
		);

		void setup_parser(
			Parser& parser,
			std::unordered_map<std::string, Node> nodesRegister,
			std::vector<ParserFlags> flags = std::vector<ParserFlags>({ ParserFlags::OPTI_CONST_EXPR })
		);

		bool has_flag(
			Parser& parser,
			ParserFlags flag
		);

		std::shared_ptr<daedalus::ast::Statement> parse_statement(
			Parser& parser,
			std::vector<daedalus::lexer::Token>& tokens
		);

		void parse(
			Parser& parser,
			std::shared_ptr<daedalus::ast::Scope> program,
			std::vector<daedalus::lexer::Token>& tokens
		);
	}
}

#endif // __DAEDALUS_PARSER__