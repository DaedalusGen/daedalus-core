#include <AquIce/daedalus/values.hpp>

#include <stdexcept>

#pragma region RuntimeValue

std::string daedalus::values::RuntimeValue::type() {
	return "RuntimeValue";
};
std::string daedalus::values::RuntimeValue::repr() {
	return "RuntimeValue";
}
bool daedalus::values::RuntimeValue::IsTrue() {
	return false;
}

#pragma endregion

#pragma region NullValue

daedalus::values::NullValue::NullValue() {}
std::string daedalus::values::NullValue::type() {
	return "NullValue";
}
std::string daedalus::values::NullValue::repr() {
	return "null";
}
bool daedalus::values::NullValue::IsTrue() {
	return false;
}

daedalus::values::NumberValue::NumberValue(double value) :
	value(value)
{}

double daedalus::values::NumberValue::get() {
	return this->value;
}
std::string daedalus::values::NumberValue::type() {
	return "NumberValue";
}
std::string daedalus::values::NumberValue::repr() {
	return std::to_string(this->value);
}
bool daedalus::values::NumberValue::IsTrue() {
	return this->value != 0;
}

#pragma endregion