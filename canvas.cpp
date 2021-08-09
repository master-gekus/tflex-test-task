#include "canvas.h"

#include <cmath>

#include <algorithm>
#include <map>
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
	class pixel_range
	{
	public:
		inline pixel_range(int x) noexcept
			: left_ {x}
			, right_ {x}
		{
		}

		pixel_range(const pixel_range&) = default;
		pixel_range(pixel_range&&) = default;
		pixel_range& operator=(const pixel_range&) = default;
		pixel_range& operator=(pixel_range&&) = default;

		[[nodiscard]] int left() const noexcept
		{
			return left_;
		}

		[[nodiscard]] int right() const noexcept
		{
			return right_;
		}

		inline pixel_range& operator+=(int x) noexcept
		{
			if (left_ > x)
			{
				left_ = x;
			}

			if (right_ < x)
			{
				right_ = x;
			}
			return *this;
		}

	private:
		int left_;
		int right_;
	};
	using pixel_ranges = std::map<int, pixel_range>;

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

	inline void draw_range(int xleft, int xright, int y, color c)
	{
		for (int x = xleft; x <= xright; ++x)
		{
			set_pixel(x, y, c);
		}
	}

	inline void draw_ranges(const pixel_ranges& ranges, color c)
	{
		for (const auto& r : ranges)
		{
			draw_range(r.second.left(), r.second.right(), r.first, c);
		}
	}

	static void line_(pixel_ranges& ranges, int x1, int y1, int x2, int y2)
	{
		int dx {std::abs(x2 - x1)};
		int sx {(x1 < x2) ? 1 : -1};
		int dy {-std::abs(y2 - y1)};
		int sy {(y1 < y2) ? 1 : -1};
		int err {dx + dy};

		while (true)
		{
			if (auto res {ranges.try_emplace(y1, x1)}; !res.second)
			{
				res.first->second += x1;
			}

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
	data::pixel_ranges ranges;
	data::line_(ranges, x1, y1, x2, y2);
	data_->draw_ranges(ranges, c);
}

void canvas::triangle(int x1, int y1, int x2, int y2, int x3, int y3, color c)
{
	data::pixel_ranges ranges;
	data::line_(ranges, x1, y1, x2, y2);
	data::line_(ranges, x1, y1, x3, y3);
	data::line_(ranges, x2, y2, x3, y3);
	data_->draw_ranges(ranges, c);
}

void canvas::ellipse(int x0, int y0, int x1, int y1, color c, bool fill)
{
	// This algorithm was just taken from:
	// https://stackoverflow.com/questions/2914807/plot-ellipse-from-rectangle
	// It doesn't work quite correctly (at least, it seems to me), but I have
	// neither the time nor the desire to understand it in detail.

	// Calculate height
	int yb {(y0 + y1) / 2};
	int yc {yb};
	int qb {(y0 < y1) ? (y1 - y0) : (y0 - y1)};
	int qy {qb};
	int dy {qb / 2};
	if (0 != (qb % 2))
	{ // Bounding box has even pixel height
		++yc;
	}

	// Calculate width
	int xb {(x0 + x1) / 2};
	int xc {xb};
	int qa {(x0 < x1) ? (x1 - x0) : (x0 - x1)};
	int qx {qa % 2};
	int dx {0};
	int qt {(qa * qa) + (qb * qb) - (2 * qa * qa * qb)};
	if (0 != qx)
	{ // Bounding box has even pixel width
		++xc;
		qt += 3L * qb * qb;
	}

	while ((qy >= 0) && (qx <= qa))
	{
		if (!fill)
		{
			data_->set_pixel(xb - dx, yb - dy, c);
			if (dx != 0 || xb != xc)
			{
				data_->set_pixel(xc + dx, yb - dy, c);
				if (dy != 0 || yb != yc)
					data_->set_pixel(xc + dx, yc + dy, c);
			}
			if (dy != 0 || yb != yc)
			{
				data_->set_pixel(xb - dx, yc + dy, c);
			}
		}

		if (((qt + (2 * qb * qb * qx) + (3 * qb * qb)) <= 0)
			|| ((qt + (2 * qa * qa * qy) - (qa * qa)) <= 0))
		{
			qt += ((8 * qb * qb) + (4 * qb * qb * qx));
			dx++;
			qx += 2;
		}
		else if ((qt - (2 * qa * qa * qy) + (3 * qa * qa)) > 0)
		{
			if (fill)
			{
				data_->draw_range(xb - dx, xc + dx, yc + dy, c);
				if ((dy != 0) || (yb != yc))
				{
					data_->draw_range(xb - dx, xc + dx, yb - dy, c);
				}
			}
			qt += ((8 * qa * qa) - (4 * qa * qa * qy));
			dy--;
			qy -= 2;
		}
		else
		{
			if (fill)
			{
				data_->draw_range(xb - dx, xc + dx, yc + dy, c);
				if (dy != 0 || yb != yc)
				{
					data_->draw_range(xb - dx, xc + dx, yb - dy, c);
				}
			}
			qt += ((8 * qb * qb) + (4 * qb * qb * qx) + (8 * qa * qa) - (4 * qa * qa * qy));
			dx++;
			qx += 2;
			dy--;
			qy -= 2;
		}
	}
}
