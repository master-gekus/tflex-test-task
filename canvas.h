#ifndef CANVAS_H
#define CANVAS_H

#include <memory>

#include "color.h"

class canvas
{
public:
	canvas(int w, int h, color c);

public:
	[[nodiscard]] inline auto width() const noexcept
	{
		return width_;
	}

	[[nodiscard]] inline auto height() const noexcept
	{
		return height_;
	}

public:
	void set_pixel(int x, int y, color c);
	color get_pixel(int x, int y) const;

private:
	int width_;
	int height_;
	std::unique_ptr<color[]> pixels_;
};

#endif // CANVAS_H
