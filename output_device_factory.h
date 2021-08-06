#ifndef OUTPUT_DEVICE_FACTORY_H
#define OUTPUT_DEVICE_FACTORY_H

#include <string>
#include <sstream>
#include <map>
#include <functional>

class output_device_interface;

class output_device_factory
{
public:
	using create_fn = std::function<output_device_interface*(std::stringstream&)>;

public:
	static output_device_interface* create(std::string name, std::stringstream& parameters);

public:
	class registrar
	{
	public:
		registrar(std::string name, create_fn creator);
	};

private:
	static std::map<std::string, create_fn> devices_;
};

#endif // OUTPUT_DEVICE_FACTORY_H
