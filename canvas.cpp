#include "canvas.h"

#include <algorithm>
#include <stdexcept>

canvas::canvas(int w, int h, color c)
	: width_ {w}
	, height_ {h}
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

void canvas::box(int x1, int y1, int x2, int y2, color c)
{
	int xmin {std::min(x1, x2)};
	int xmax {std::max(x1, x2)};
	int ymin {std::min(y1, y2)};
	int ymax {std::max(y1, y2)};

	for (int y = ymin; y <= ymax; ++y)
	{
		for (int x = xmin; x <= xmax; ++x)
		{
			set_pixel(x, y, c);
		}
	}
}
