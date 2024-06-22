#include <AquIce/daedalus/ast.hpp>

std::string daedalus::ast::Statement::type() {
	return "Statement";
}
std::string daedalus::ast::Statement::repr(int indent) {
	return std::string(indent, '\t') + "Statement";
}

daedalus::ast::Scope::Scope(std::vector<std::shared_ptr<daedalus::ast::Statement>> body) {
	this->body = body;
}
			
std::string daedalus::ast::Scope::type() {
	return "Scope";
}
std::string daedalus::ast::Scope::repr(int indent) {
	std::string pretty = std::string(indent, '\t') + "{\n";

	for(std::shared_ptr<daedalus::ast::Statement> statement : this->body) {
		pretty += statement->repr(indent + 1) + "\n";
	}

	pretty += std::string(indent, '\t') + "\n}";

	return pretty;
}

std::shared_ptr<daedalus::ast::Expression> daedalus::ast::Expression::get_constexpr() {
	return nullptr;
}

daedalus::ast::NumberExpression::NumberExpression(double value) {
	this->value = value;
}
std::string daedalus::ast::NumberExpression::type() {
	return "NumberExpression";
}
std::shared_ptr<daedalus::ast::Expression> daedalus::ast::NumberExpression::get_constexpr() {
	return this->shared_from_this();
}
std::string daedalus::ast::NumberExpression::repr(int indent) {
	return std::string(indent, '\t') + "NumberExpression(" + std::to_string(this->value) + ")";
}