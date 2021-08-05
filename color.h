#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

#include <iostream>
#include <string>
#include <type_traits>

class color
{
public:
	color() = default;

	constexpr color(uint8_t red, uint8_t green, uint8_t blue) noexcept
		: value_ {(static_cast<uint32_t>(red) << 16u) | (static_cast<uint32_t>(green) << 8u) | static_cast<uint32_t>(blue)}
	{
	}

	explicit color(std::string color_name);

public:
	constexpr inline uint8_t red() const noexcept
	{
		return static_cast<uint8_t>(value_ >> 16u);
	}

	constexpr inline uint8_t green() const noexcept
	{
		return static_cast<uint8_t>(value_ >> 8u);
	}

	constexpr inline uint8_t blue() const noexcept
	{
		return static_cast<uint8_t>(value_);
	}

	std::string to_string() const;

private:
	uint32_t value_ {0};
};

static_assert(std::is_trivially_copy_constructible_v<color>);
static_assert(std::is_trivially_copy_assignable_v<color>);
static_assert(std::is_trivially_move_constructible_v<color>);
static_assert(std::is_trivially_move_assignable_v<color>);

inline std::ostream& operator<<(std::ostream& os, color c)
{
	return os << c.to_string();
}

inline std::istream& operator>>(std::istream& is, color& c)
{
	std::string str_color;
	is >> str_color;
	c = color {str_color};
	return is;
}

#endif // COLOR_H
