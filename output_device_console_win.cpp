#include "output_device_console_win.h"

#include "color.h"

output_device_factory::registrar output_device_console_win::registrar_ {
	"console", []([[maybe_unused]] std::stringstream& paramters) {
		return new output_device_console_win {};
	}};

void output_device_console_win::save_console_attr()
{

}

void output_device_console_win::restore_console_attr()
{

}

void output_device_console_win::set_console_attr(color c)
{
}
