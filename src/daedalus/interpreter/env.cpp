#include <AquIce/daedalus/env.hpp>

daedalus::env::Environment::Environment(
	std::vector<std::string> envValuesProperties,
	std::vector<EnvValidationRule> validationRules,
	std::shared_ptr<Environment> parent
) {
	this->envValuesProperties = envValuesProperties;
	this->validationRules = validationRules;
	this->parent = parent;
	this->values = std::unordered_map<std::string, EnvValue>();
}

bool daedalus::env::Environment::has_value(std::string key) {
	return this->values.find(key) != this->values.end();
}

std::shared_ptr<daedalus::values::RuntimeValue> daedalus::env::Environment::set_value(
	std::string key,
	std::shared_ptr<daedalus::values::RuntimeValue> value
) {
	if(!this->has_value(key)) {
		if(this->parent != nullptr) {
			return this->parent->set_value(key, value);
		}
		throw std::runtime_error("Trying to set non-declared variable " + key);
	}
	
	std::shared_ptr<daedalus::values::RuntimeValue> old_val = this->get_value(key);

	for(const daedalus::env::EnvValidationRule& rule : this->validationRules) {
		if(std::find(rule.sensitivity.begin(), rule.sensitivity.end(), daedalus::env::ValidationRuleSensitivity::SET) != rule.sensitivity.end()) {
			this->values.at(key) = rule.validationFunction(
				this->values.at(key),
				old_val
			);
			return old_val;
		}
	}

	this->values.at(key).value = value;

	return old_val;
}

std::shared_ptr<daedalus::values::RuntimeValue> daedalus::env::Environment::init_value(
	std::string key,
	std::shared_ptr<daedalus::values::RuntimeValue> value,
	std::unordered_map<std::string, std::string> properties
) {

	for(const auto& [prop_key, prop_value] : properties) {
		if(std::find(this->envValuesProperties.begin(), this->envValuesProperties.end(), prop_key) == this->envValuesProperties.end()) {
			throw std::runtime_error("Invalid variable property " + prop_key);
		}
	}

	if(this->has_value(key)) {
		throw std::runtime_error("Trying to redeclare an existing variable " + key);
	}

	for(const daedalus::env::EnvValidationRule& rule : this->validationRules) {
		if(std::find(rule.sensitivity.begin(), rule.sensitivity.end(), daedalus::env::ValidationRuleSensitivity::INIT) != rule.sensitivity.end()) {
			this->values[key] = rule.validationFunction(
				daedalus::env::EnvValue(daedalus::env::EnvValue{value, properties}),
				nullptr
			);
			return value;
		}
	}

	return value;
}

std::shared_ptr<daedalus::values::RuntimeValue> daedalus::env::Environment::get_value(std::string key) {
	if(!this->has_value(key)) {
		if(this->parent != nullptr) {
			return this->parent->get_value(key);
		}
		throw std::runtime_error("Trying to get non-declared variable " + key);
	}
	for(const daedalus::env::EnvValidationRule& rule : this->validationRules) {
		if(std::find(rule.sensitivity.begin(), rule.sensitivity.end(), daedalus::env::ValidationRuleSensitivity::GET) != rule.sensitivity.end()) {
			return rule.validationFunction(
				this->values.at(key),
				nullptr
			).value;
		}
	}
	return this->values.at(key).value;
}