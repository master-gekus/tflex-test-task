#include "output_device_console.h"

#include <iostream>

#include "canvas.h"
#include "color.h"

void output_device_console::output_canvas(const canvas* c)
{
	save_console_attr();

	for (int y = 0; y < c->height(); ++y)
	{
		for (int x = 0; x < c->width(); ++x)
		{
			set_console_attr(c->get_pixel(x, y));
			std::cout << '*';
		}
		std::cout << std::endl;
	}

	restore_console_attr();
}
