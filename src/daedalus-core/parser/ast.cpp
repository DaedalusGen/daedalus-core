#include <daedalus/core/parser/ast.hpp>
#include <memory>

std::string daedalus::core::ast::Statement::type() {
	return "Statement";
}
std::string daedalus::core::ast::Statement::repr(int indent) {
	return std::string(indent, '\t') + "Statement";
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::core::ast::Expression::get_constexpr() {
	return nullptr;
}

daedalus::core::ast::Scope::Scope(std::vector<std::shared_ptr<daedalus::core::ast::Expression>> body) :
	body(body)
{}

std::vector<std::shared_ptr<daedalus::core::ast::Expression>> daedalus::core::ast::Scope::get_body() {
    return this->body;
}
void daedalus::core::ast::Scope::push_back_body(std::shared_ptr<Expression> expression) {
    this->body.push_back(expression);
}

std::string daedalus::core::ast::Scope::type() {
	return "Scope";
}
std::shared_ptr<daedalus::core::ast::Expression> daedalus::core::ast::Scope::get_constexpr() {
    auto body = std::vector<std::shared_ptr<daedalus::core::ast::Expression>>();
    for(const std::shared_ptr<daedalus::core::ast::Expression> expression : this->body) {
        body.push_back(expression->get_constexpr());
    }
    this->body = body;
    return this->shared_from_this();
}
std::string daedalus::core::ast::Scope::repr(int indent) {
	std::string pretty = std::string(indent, '\t') + "{\n";

	for(std::shared_ptr<daedalus::core::ast::Expression> expression : this->body) {
		pretty += expression->repr(indent + 1) + "\n";
	}

	pretty += std::string(indent, '\t') + "}";

	return pretty;
}

daedalus::core::ast::NumberExpression::NumberExpression(double value) :
	value(value)
{}

double daedalus::core::ast::NumberExpression::get_value() {
    return this->value;
}
void daedalus::core::ast::NumberExpression::set_value(double value) {
    this->value = value;
}

std::string daedalus::core::ast::NumberExpression::type() {
	return "NumberExpression";
}
std::shared_ptr<daedalus::core::ast::Expression> daedalus::core::ast::NumberExpression::get_constexpr() {
	return this->shared_from_this();
}
std::string daedalus::core::ast::NumberExpression::repr(int indent) {
	return std::string(indent, '\t') + "NumberExpression(" + std::to_string(this->value) + ")";
}
