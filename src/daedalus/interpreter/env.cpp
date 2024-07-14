#include <AquIce/daedalus/env.hpp>

daedalus::env::Environment::Environment(
	std::vector<std::string> envValuesProperties,
	std::vector<EnvValidationRule> validationRules,
	std::shared_ptr<Environment> parent
) :
	envValuesProperties(envValuesProperties),
	validationRules(validationRules),
	parent(parent),
	values()
{}

bool daedalus::env::Environment::has_value(std::string key) {
	return this->values.find(key) != this->values.end();
}

std::shared_ptr<daedalus::values::RuntimeValue> daedalus::env::Environment::set_value(
	std::string key,
	std::shared_ptr<daedalus::values::RuntimeValue> value
) {
	if(!this->has_value(key)) {
		DAE_ASSERT_TRUE(
			this->parent != nullptr,
			std::runtime_error("Trying to set non-declared variable " + key)
		)
		return this->parent->set_value(key, value);
	}
	
	std::shared_ptr<daedalus::values::RuntimeValue> oldVal = this->get_value(key);

	daedalus::env::EnvValue envValue = daedalus::env::EnvValue{
		value,
		this->values.at(key).properties
	};

	for(const daedalus::env::EnvValidationRule& rule : this->validationRules) {
		if(std::find(rule.sensitivity.begin(), rule.sensitivity.end(), daedalus::env::ValidationRuleSensitivity::SET) != rule.sensitivity.end()) {
			envValue = rule.validationFunction(
				this->values.at(key),
				envValue.value,
				key
			);
		}
	}

	this->values.at(key) = envValue;

	return oldVal;
}

std::shared_ptr<daedalus::values::RuntimeValue> daedalus::env::Environment::init_value(
	std::string key,
	std::shared_ptr<daedalus::values::RuntimeValue> value,
	std::unordered_map<std::string, std::string> properties
) {

	for(const auto& [prop_key, prop_value] : properties) {
		DAE_ASSERT_TRUE(
			std::find(this->envValuesProperties.begin(), this->envValuesProperties.end(), prop_key) != this->envValuesProperties.end(),
			std::runtime_error("Invalid variable property " + prop_key)
		)
	}

	DAE_ASSERT_TRUE(
		!this->has_value(key),
		std::runtime_error("Trying to redeclare an existing variable " + key)
	)

	daedalus::env::EnvValue envValue = daedalus::env::EnvValue{
		value,
		properties
	};

	for(const daedalus::env::EnvValidationRule& rule : this->validationRules) {
		if(std::find(rule.sensitivity.begin(), rule.sensitivity.end(), daedalus::env::ValidationRuleSensitivity::INIT) != rule.sensitivity.end()) {
			envValue = rule.validationFunction(
				envValue,
				nullptr,
				key
			);
		}
	}

	this->values[key] = envValue;

	return value;
}

std::shared_ptr<daedalus::values::RuntimeValue> daedalus::env::Environment::get_value(std::string key) {
	if(!this->has_value(key)) {
		DAE_ASSERT_TRUE(
			this->parent != nullptr,
			std::runtime_error("Trying to get non-declared variable " + key)
		)
		return this->parent->get_value(key);
		
	}

	daedalus::env::EnvValue envValue = this->values.at(key);

	for(const daedalus::env::EnvValidationRule& rule : this->validationRules) {
		if(std::find(rule.sensitivity.begin(), rule.sensitivity.end(), daedalus::env::ValidationRuleSensitivity::GET) != rule.sensitivity.end()) {
			envValue = rule.validationFunction(
				envValue,
				nullptr,
				key
			);
		}
	}
	return this->values.at(key).value;
}