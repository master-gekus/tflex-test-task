#include "canvas.h"

#include <cmath>

#include <algorithm>
#include <stdexcept>

#include "color.h"

class canvas::data
{
public:
	inline data(int w, int h, color c)
		: width_ {w}
		, height_ {h}
	{
		if ((1 > w) || (1920 < w) || (1 > h) || (1200 < h))
		{
			throw std::invalid_argument {"canvas::canvas(): invalid size."};
		}

		pixels_.reset(new color[w * h] {c});
	}

public:
	[[nodiscard]] inline int width() const noexcept
	{
		return width_;
	}

	[[nodiscard]] inline int height() const noexcept
	{
		return height_;
	}

	inline void set_pixel(int x, int y, color c)
	{
		if ((0 > x) || (width_ <= x) || (0 > y) || (height_ <= y))
		{
			return;
		}

		pixels_[(y * width_) + x] = c;
	}

	inline color get_pixel(int x, int y) const
	{
		if ((0 > x) || (width_ <= x) || (0 > y) || (height_ <= y))
		{
			throw std::invalid_argument {"canvas::get_pixel(): bad coordinates."};
		}

		return pixels_[(y * width_) + x];
	}

private:
	int width_;
	int height_;
	std::unique_ptr<color[]> pixels_;
};

canvas::canvas(int w, int h, color c)
	: data_ {new data {w, h, c}}
{
}

canvas::~canvas()
{
}

int canvas::width() const
{
	return data_->width();
}

int canvas::height() const
{
	return data_->height();
}

void canvas::set_pixel(int x, int y, color c)
{
	data_->set_pixel(x, y, c);
}

color canvas::get_pixel(int x, int y) const
{
	return data_->get_pixel(x, y);
}

void canvas::box(int x1, int y1, int x2, int y2, color c, bool filled)
{
	int xmin {std::min(x1, x2)};
	int xmax {std::max(x1, x2)};
	int ymin {std::min(y1, y2)};
	int ymax {std::max(y1, y2)};

	for (int y = ymin; y <= ymax; ++y)
	{
		for (int x = xmin; x <= xmax; ++x)
		{
			if (filled || (x == x1) || (x == x2) || (y == y1) || (y == y2))
			{
				set_pixel(x, y, c);
			}
		}
	}
}

void canvas::line(int x1, int y1, int x2, int y2, color c)
{
	int dx {std::abs(x2 - x1)};
	int sx {(x1 < x2) ? 1 : -1};
	int dy {-std::abs(y2 - y1)};
	int sy {(y1 < y2) ? 1 : -1};
	int err {dx + dy};

	while (true)
	{
		set_pixel(x1, y1, c);
		if ((x1 == x2) && (y1 == y2))
		{
			break;
		}

		int e2 {2 * err};
		if (e2 >= dy)
		{
			err += dy;
			x1 += sx;
		}

		if (e2 <= dx)
		{
			err += dx;
			y1 += sy;
		}
	}
}
