#ifndef __DAEDALUS_LEXER__
#define __DAEDALUS_LEXER__

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <functional>

/**
 * Peek at the first character a string
 * @param str The string to peek
 * @return The first character
 * @note This function uses `[[nodiscard]]`, you need to explicitly cast the value to `void` if not used
 */
[[nodiscard]] char peek(std::string str);

/**
 * Shift the first character a string
 * @param str The string to shift
 * @return The shifted character
 * @note This function uses `[[nodiscard]]`, you need to explicitly cast the value to `void` if not used
 */
[[nodiscard]] std::string shift(std::string& str);

/**
 * Shift the `n` first characters of a string
 * @param str The string to shift
 * @param n The number of characters to shift
 * @return The shifted string
 * @note This function uses `[[nodiscard]]`, you need to explicitly cast the value to `void` if not used
 */
[[nodiscard]] std::string shift(std::string& str, int n);

/**
 * Check if a string stats with a substring
 * @param str The string to check in
 * @param substr The substring to look for
 * @return Whether `str` starts with `substr`
 */
bool startswith(std::string str, std::string substr);

namespace daedalus {
	namespace lexer {

		typedef struct TokenType {
			std::string name;
			/**
			 * The function to call to lex the token
			 * @param src The source string to lex
			 * @return The value of the lexed token (or an empty string if the token is not found)
			 */
			std::function<std::string (std::string src)> lex_token;
		} TokenType;

		/**
		 * A lexer configuration
		 */
		typedef struct Lexer {
			std::vector<TokenType> tokenTypes;
			std::vector<char> whitespaces = std::vector<char>({
				' ',
				'\t',
				'\n',
				'\r',
			});
			std::string singleLineComment = "//";
			std::pair<std::string, std::string> multiLineComment = { "/*", "*/" };
			char decimalSeparator = '.';
			char charDelimiter = '\'';
			char stringDelimiter = '"';
			char escapeCharacter = '\\';
		} Lexer;

		/**
		 * A token
		 */
		typedef struct Token {
			std::string type;
			std::string value;
		} Token;

		/**
		 * Get the string representation of a token
		 * @param token The token to get the representation of
		 * @return The string representation
		 */
		std::string repr(const Token& token);

		/**
		 * Create a token with the same name and value
		 * @param name The name and value of the token
		 * @return The created token
		 */
		TokenType make_token_type(std::string name);

		/**
		 * Create a token with a name and a value
		 * @param name The name of the token
		 * @param value The value of the token
		 * @return The created token
		 */
		TokenType make_token_type(std::string name, std::string value);

		/**
		 * Create a token with a name and a lexing function
		 * @param name The name of the token
		 * @param lex_token The function to run to lex the token
		 * @return The created token
		 */
		TokenType make_token_type(std::string name, std::function<std::string(std::string)> lex_token);

		/**
		 * Update the configuration of a lexer
		 */
		void setup_lexer(
			Lexer& lexer,
			const std::vector<TokenType>& tokenTypes,
			std::vector<char> whitespaces = std::vector<char>({
				' ',
				'\t',
				'\n',
				'\r',
			}),
			std::string singleLineComment = "//",
			std::pair<std::string, std::string> multiLineComment = { "/*", "*/" },
			char decimalSeparator = '.',
			char charDelimiter = '\'',
			char stringDelimiter = '"',
			char escapeCharacter = '\\'
		);

		/**
		 * Lex a source string into a vector of tokens
		 * @param lexer The lexer to use the configuation of
		 * @param tokens A reference to the vector of tokens to fill
		 * @param src The source string
		 */
		void lex(
			Lexer& lexer,
			std::vector<Token>& tokens,
			std::string src
		);
	}
}

#endif // __DAEDALUS_LEXER__