#ifndef CANVAS_H
#define CANVAS_H

#include <memory>

class color;

class canvas final
{
public:
	canvas(int w, int h, color c);
	~canvas();

public:
	[[nodiscard]] int width() const;
	[[nodiscard]] int height() const;

public:
	void set_pixel(int x, int y, color c);
	[[nodiscard]] color get_pixel(int x, int y) const;

	void box(int x1, int y1, int x2, int y2, color c, bool filled);
	void line(int x1, int y1, int x2, int y2, color c);
	void triangle(int x1, int y1, int x2, int y2, int x3, int y3, color c);

private:
	class data;
	std::unique_ptr<data> data_;
};

#endif // CANVAS_H
