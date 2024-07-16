#include <AquIce/daedalus/values.hpp>

#pragma region RuntimeValue

std::string daedalus::core::values::RuntimeValue::type() {
	return "RuntimeValue";
};
std::string daedalus::core::values::RuntimeValue::repr() {
	return "RuntimeValue";
}
bool daedalus::core::values::RuntimeValue::IsTrue() {
	return false;
}

#pragma endregion

#pragma region NullValue

daedalus::core::values::NullValue::NullValue() {}
std::string daedalus::core::values::NullValue::type() {
	return "NullValue";
}
std::string daedalus::core::values::NullValue::repr() {
	return "null";
}
bool daedalus::core::values::NullValue::IsTrue() {
	return false;
}

daedalus::core::values::NumberValue::NumberValue(double value) :
	value(value)
{}

double daedalus::core::values::NumberValue::get() {
	return this->value;
}
std::string daedalus::core::values::NumberValue::type() {
	return "NumberValue";
}
std::string daedalus::core::values::NumberValue::repr() {
	return std::to_string(this->value);
}
bool daedalus::core::values::NumberValue::IsTrue() {
	return this->value != 0;
}

#pragma endregion
