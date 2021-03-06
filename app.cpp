#include "app.h"

#include <cctype> // for std::toupper
#include <cstring> // for std::strncmp, std::strlen

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <string>

#include "canvas.h"
#include "color.h"
#include "output_device_factory.h"
#include "output_device_interface.h"

struct app::cmd_entry
{
	const char* cmd_ {nullptr};
	void (app::*method_)(std::stringstream&) {nullptr};
	const char* desc_ {nullptr};
	const char* args_ {nullptr};
};

int app::exec() noexcept
{
	try
	{
		while (!std::cin.eof())
		{
			std::string line;
			std::getline(std::cin, line);
			std::for_each(line.begin(), line.end(), [](auto& c) { c = std::toupper(c); });

			std::stringstream command_line {line};
			std::string command;
			command_line >> command;
			if (command.empty() || (';' == command[0]))
			{
				continue;
			}

			if (const auto* cmd {look_for_command(command.c_str())}; nullptr != cmd)
			{
				try
				{
					(this->*cmd->method_)(command_line);
				}
				catch (const std::exception& e)
				{
					std::cout << "Error executing command \"" << cmd->cmd_ << "\": " << e.what() << std::endl;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cout << "Unknown exception." << std::endl;
		return 1;
	}

	return 0;
}

void app::cmd_canvas(std::stringstream& parameters)
{
	int w {0};
	int h {0};
	color c {};
	parameters >> w >> h >> c;

	canvas_.reset(new canvas {w, h, c});
}

void app::cmd_pixel(std::stringstream& parameters)
{
	check_canvas();

	int x {0};
	int y {0};
	color c {};
	parameters >> x >> y >> c;

	canvas_->set_pixel(x, y, c);
}

void app::cmd_box(std::stringstream& parameters)
{
	check_canvas();

	int x1 {0};
	int y1 {0};
	int x2 {0};
	int y2 {0};
	color c {};
	parameters >> x1 >> y1 >> x2 >> y2 >> c;

	canvas_->box(x1, y1, x2, y2, c, false);
}

void app::cmd_bar(std::stringstream& parameters)
{
	check_canvas();

	int x1 {0};
	int y1 {0};
	int x2 {0};
	int y2 {0};
	color c {};
	parameters >> x1 >> y1 >> x2 >> y2 >> c;

	canvas_->box(x1, y1, x2, y2, c, true);
}

void app::cmd_ellipse(std::stringstream& parameters)
{
	check_canvas();

	int x1 {0};
	int y1 {0};
	int x2 {0};
	int y2 {0};
	color c {};
	parameters >> x1 >> y1 >> x2 >> y2 >> c;

	canvas_->ellipse(x1, y1, x2, y2, c, false);
}

void app::cmd_fellipse(std::stringstream& parameters)
{
	check_canvas();

	int x1 {0};
	int y1 {0};
	int x2 {0};
	int y2 {0};
	color c {};
	parameters >> x1 >> y1 >> x2 >> y2 >> c;

	canvas_->ellipse(x1, y1, x2, y2, c, true);
}

void app::cmd_line(std::stringstream& parameters)
{
	check_canvas();

	int x1 {0};
	int y1 {0};
	int x2 {0};
	int y2 {0};
	color c {};
	parameters >> x1 >> y1 >> x2 >> y2 >> c;

	canvas_->line(x1, y1, x2, y2, c);
}

void app::cmd_triangle(std::stringstream& parameters)
{
	int x1 {0};
	int y1 {0};
	int x2 {0};
	int y2 {0};
	int x3 {0};
	int y3 {0};
	color c {};
	parameters >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> c;

	canvas_->triangle(x1, y1, x2, y2, x3, y3, c);
}

void app::cmd_output(std::stringstream& parameters)
{
	check_canvas();

	std::string device_name;
	parameters >> device_name;

	std::unique_ptr<output_device_interface> device {output_device_factory::create(device_name, parameters)};

	device->output_canvas(canvas_.get());
}

const app::cmd_entry* app::commands() noexcept
{
	static const constexpr cmd_entry commands_[] = {
		{"HELP", &app::cmd_help,
		 "Print this help screen", "<X> <Y> <COLOR>"},

		{"?", &app::cmd_help,
		 "Same as \"HELP\""},

		{"CANVAS", &app::cmd_canvas,
		 "Create canvas of size <W>x<H>", "<W> <H> [<BACKGROUN-COLOR>]"},

		{"PIXEL", &app::cmd_pixel,
		 "Set pixel (<X>,<Y>) to color <COLOR>", "<X> <Y> [<COLOR>]"},

		{"BAR", &app::cmd_bar,
		 "Draws bar (filled rectangle) from (<X1>,<Y1>) to (<X2>,<Y2>) of color <COLOR>",
		 "<X1> <Y1> <X2> <Y2> [<COLOR>]"},

		{"BOX", &app::cmd_box,
		 "Draws box (non-filled rectangle) from (<X1>,<Y1>) to (<X2>,<Y2>) of color <COLOR>",
		 "<X1> <Y1> <X2> <Y2> [<COLOR>]"},

		{"LINE", &app::cmd_line,
		 "Draws line from (<X1>,<Y1>) to (<X2>,<Y2>) of color <COLOR>",
		 "<X1> <Y1> <X2> <Y2> [<COLOR>]"},

		{"TRIANGLE", &app::cmd_triangle,
		 "Draws filled triangle with agles in (<X1>,<Y1>), (<X2>,<Y2>), (<X3>,<Y3>) of color <COLOR>",
		 "<X1> <Y1> <X2> <Y2> <X3> <Y3> [<COLOR>]"},

		{"ELLIPSE", &app::cmd_ellipse,
		 "Draws non-filled ellipse bounded by rectangle from (<X1>,<Y1>) to (<X2>,<Y2>) of color <COLOR>",
		 "<X1> <Y1> <X2> <Y2> [<COLOR>]"},

		{"FELLIPSE", &app::cmd_fellipse,
		 "Draws filled ellipse bounded by rectangle from (<X1>,<Y1>) to (<X2>,<Y2>) of color <COLOR>",
		 "<X1> <Y1> <X2> <Y2> [<COLOR>]"},

		{"OUTPUT", &app::cmd_output,
		 "Output canvas to device <DEVICE> with optional <PARAMS>", "<DEVICE> [<PARAMS>]"},

		{}};
	return commands_;
}

const app::cmd_entry* app::look_for_command(const char* cmd) noexcept
{
	const auto cmdlen {std::strlen(cmd)};
	std::list<const app::cmd_entry*> ac;
	for (const auto* c {commands()}; nullptr != c->cmd_; ++c)
	{
		if (0 == std::strncmp(cmd, c->cmd_, cmdlen))
		{
			ac.emplace_back(c);
		}
	}

	if (ac.empty())
	{
		std::cout << "Unknown command: \"" << cmd << "\"" << std::endl;
		std::cout << "Type \"HELP\" for list of available commands." << std::endl;
		return nullptr;
	}

	if (1 < ac.size())
	{
		std::cout << "Ambiguous command: \"" << cmd << "\"" << std::endl;
		std::cout << "May be interpreted as: ";
		for (const auto& c : ac)
		{
			std::cout << "\"" << c->cmd_ << "\" ";
		}
		std::cout << std::endl;
		return nullptr;
	}

	return ac.front();
}

void app::cmd_help([[maybe_unused]] std::stringstream& parameters)
{
	std::cout << "Available commands:" << std::endl;
	for (const auto* c {commands()}; nullptr != c->cmd_; ++c)
	{
		std::cout << " - " << c->cmd_;
		if (nullptr != c->args_)
		{
			std::cout << " " << c->args_;
		}
		std::cout << std::endl;

		if (nullptr != c->desc_)
		{
			std::cout << "     " << c->desc_ << std::endl;
		}
		std::cout << std::endl;
	}
}

inline void app::check_canvas()
{
	if (!canvas_)
	{
		throw std::runtime_error("Canvas not created.");
	}
}
