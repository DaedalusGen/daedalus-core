#include <daedalus/core/parser/ast.hpp>

std::string daedalus::core::ast::Statement::type() {
	return "Statement";
}
std::string daedalus::core::ast::Statement::repr(int indent) {
	return std::string(indent, '\t') + "Statement";
}

daedalus::core::ast::Scope::Scope(std::vector<std::shared_ptr<daedalus::core::ast::Expression>> body) :
	body(body)
{}

std::string daedalus::core::ast::Scope::type() {
	return "Scope";
}
std::string daedalus::core::ast::Scope::repr(int indent) {
	std::string pretty = std::string(indent, '\t') + "{\n";

	for(std::shared_ptr<daedalus::core::ast::Expression> statement : this->body) {
		pretty += statement->repr(indent + 1) + "\n";
	}

	pretty += std::string(indent, '\t') + "\n}";

	return pretty;
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::core::ast::Expression::get_constexpr() {
	return nullptr;
}

daedalus::core::ast::NumberExpression::NumberExpression(double value) :
	value(value)
{}

std::string daedalus::core::ast::NumberExpression::type() {
	return "NumberExpression";
}
std::shared_ptr<daedalus::core::ast::Expression> daedalus::core::ast::NumberExpression::get_constexpr() {
	return this->shared_from_this();
}
std::string daedalus::core::ast::NumberExpression::repr(int indent) {
	return std::string(indent, '\t') + "NumberExpression(" + std::to_string(this->value) + ")";
}
