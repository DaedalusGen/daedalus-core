#ifndef __DAEDALUS_CORE_PARSER__
#define __DAEDALUS_CORE_PARSER__

#include <daedalus/core/lexer/lexer.hpp>
#include <daedalus/core/parser/ast.hpp>
#include <daedalus/core/tools/assert.hpp>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

[[nodiscard]] daedalus::core::lexer::Token peek(std::vector<daedalus::core::lexer::Token>& tokens);

[[nodiscard]] daedalus::core::lexer::Token eat(std::vector<daedalus::core::lexer::Token>& tokens);

[[nodiscard]] daedalus::core::lexer::Token expect(std::vector<daedalus::core::lexer::Token>& tokens, std::string tokenType, std::runtime_error error);

namespace daedalus {
    namespace core {
    	namespace parser {

    		typedef struct Parser Parser;

    		typedef std::function<std::shared_ptr<daedalus::core::ast::Statement> (Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens)> ParseNodeFunction;

    		typedef struct Node {
    			ParseNodeFunction parse_node;
    			bool isTopNode;
    		} Node;

    		enum class ParserFlags {
    			OPTI_CONST_EXPR,
    		};

    		struct Parser {
    			std::unordered_map<std::string, Node> nodesRegister;
    			std::vector<ParserFlags> flags;
    		};

    		Node make_node(
    			ParseNodeFunction parse_node,
    			bool isTopNode = true
    		);

    		void demoteTopNode(
    			Parser& parser,
    			std::string key
    		);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_number_expression(Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens);

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

    		std::shared_ptr<daedalus::core::ast::Statement> parse_statement(
    			Parser& parser,
    			std::vector<daedalus::core::lexer::Token>& tokens
    		);

    		void parse(
    			Parser& parser,
    			std::shared_ptr<daedalus::core::ast::Scope> program,
    			std::vector<daedalus::core::lexer::Token>& tokens
    		);
    	}
    }
}

#endif // __DAEDALUS_CORE_PARSER__
