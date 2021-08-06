#include "output_device_factory.h"

#include <cctype> // for std::toupper

#include <algorithm>
#include <stdexcept>

std::map<std::string, output_device_factory::create_fn> output_device_factory::devices_ {};

output_device_factory::registrar::registrar(std::string name, output_device_factory::create_fn creator)
{
	std::for_each(name.begin(), name.end(), [](auto& c) { c = std::toupper(c); });
	if (!devices_.try_emplace(name, creator).second)
	{
		throw std::invalid_argument {"Output device \"" + name + "\" already registered."};
	}
}

output_device_interface* output_device_factory::create(std::string name, std::stringstream& parameters)
{
	if (name.empty())
	{
		throw std::invalid_argument {"output_device_factory::create(): empty device name"};
	}
	std::for_each(name.begin(), name.end(), [](auto& c) { c = std::toupper(c); });

	const auto it {devices_.find(name)};
	if (devices_.end() == it)
	{
		throw std::invalid_argument {"output_device_factory::create(): device \"" + name + "\" not registered."};
	}

	return it->second(parameters);
}
