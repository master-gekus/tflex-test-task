#include "output_device_console_ansi.h"

#include <iostream>

#include "color.h"

output_device_factory::registrar output_device_console_ansi::registrar_ {
	"console", []([[maybe_unused]] std::stringstream& paramters) {
		return new output_device_console_ansi {};
	}};

void output_device_console_ansi::save_console_attr()
{
}

void output_device_console_ansi::restore_console_attr()
{
	std::cout << "\x1B[0m";
	last_used_attr_ = static_cast<uint8_t>(-1);
}

void output_device_console_ansi::set_console_attr(color c)
{
	// https://en.wikipedia.org/wiki/ANSI_escape_code

	const uint8_t r {c.red()};
	const uint8_t g {c.green()};
	const uint8_t b {c.blue()};

	bool is_intensified {(r > 128) || (g > 128) || (b > 128)};
	const uint8_t limit {static_cast<uint8_t>(is_intensified ? 192 : 64)};

	uint8_t attr {0};
	if (r > limit)
	{
		attr |= 1;
	}

	if (g > limit)
	{
		attr |= 2;
	}

	if (b > limit)
	{
		attr |= 4;
	}

	if ((0 == attr) && is_intensified)
	{
		attr = 7;
		is_intensified = false;
	}
	else if ((7 == attr) && (!is_intensified))
	{
		attr = 0;
		is_intensified = true;
	}

	attr += (is_intensified ? 90 : 30);
	if (last_used_attr_ != attr)
	{
		std::cout << "\x1B[" << static_cast<unsigned>(attr) << "m";
		last_used_attr_ = attr;
	}
}
