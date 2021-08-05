#include "color.h"

#include <cctype> // for std::toupper, std::tolower
#include <cstdlib> // for std::strtoul

#include <iomanip>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace
{
constexpr struct
{
	const char* name_;
	const color color_;
} named_colors[] = {
	{"White", {255, 255, 255}}, // #FFFFFF
	{"Silver", {192, 192, 192}}, // #C0C0C0
	{"Gray", {128, 128, 128}}, // #808080
	{"Black", {0, 0, 0}}, // #000000
	{"Red", {255, 0, 0}}, // #FF0000
	{"Maroon", {128, 0, 0}}, // #800000
	{"Yellow", {255, 255, 0}}, // #FFFF00
	{"Olive", {128, 128, 0}}, // #808000
	{"Lime", {0, 255, 0}}, // #00FF00
	{"Green", {0, 128, 0}}, // #008000
	{"Aqua", {0, 255, 255}}, // #00FFFF
	{"Teal", {0, 128, 128}}, // #008080
	{"Blue", {0, 0, 255}}, // #0000FF
	{"Navy", {0, 0, 128}}, // #000080
	{"Fuchsia", {255, 0, 255}}, // #FF00FF
	{"Purple", {128, 0, 128}}, // #800080
};
} // namespace

color::color(std::string color_name)
{
	if (color_name.empty())
	{
		throw std::invalid_argument("color::color(): empty string");
	}

	for (std::size_t i = 0; i < sizeof(named_colors) / sizeof(named_colors[0]); ++i)
	{
		const char* s = color_name.c_str();
		const char* d = named_colors[i].name_;

		while ((*s) && (*s) && (std::toupper(*s) == std::toupper(*d)))
		{
			++s;
			++d;
		}
		if (('\0' == (*s)) && ('\0' == (*d)))
		{
			*this = named_colors[i].color_;
			return;
		}
	}

	static const std::regex hex_color {"#([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})", std::regex::icase};
	std::smatch m;
	if (std::regex_match(color_name, m, hex_color))
	{
		*this = color {
			static_cast<uint8_t>(std::strtoul(m[1].str().c_str(), nullptr, 16)),
			static_cast<uint8_t>(std::strtoul(m[2].str().c_str(), nullptr, 16)),
			static_cast<uint8_t>(std::strtoul(m[3].str().c_str(), nullptr, 16))};
		return;
	}

	throw std::invalid_argument("color::color(): unparsable string: \"" + color_name + "\"");
}

std::string color::to_string() const
{
	std::stringstream ss;
	ss << "#"
	   << std::setw(6) << std::setfill('0') << std::hex << std::uppercase
	   << (value_ & 0xFFFFFFu);
	return ss.str();
}
