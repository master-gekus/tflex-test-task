#include "canvas.h"

#include <stdexcept>

canvas::canvas(int w, int h, color c)
{
	if ((1 > w) || (1920 < w) || (1 > h) || (1200 < h))
	{
		throw std::invalid_argument {"canvas::canvas(): invalid size."};
	}

	pixels_.reset(new color[w * h] {c});
}

void canvas::set_pixel(int x, int y, color c)
{
	if ((0 > x) || (width_ <= x) || (0 > y) || (height_ <= y))
	{
		return;
	}

	pixels_[(y * width_) + x] = c;
}

color canvas::get_pixel(int x, int y) const
{
	if ((0 > x) || (width_ <= x) || (0 > y) || (height_ <= y))
	{
		throw std::invalid_argument {"canvas::get_pixel(): bad coordinates."};
	}

	return pixels_[(y * width_) + x];
}
