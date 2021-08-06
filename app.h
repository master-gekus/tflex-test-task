#ifndef APP_H
#define APP_H

#include <sstream>
#include <memory>

#include "canvas.h"

class app
{
public:
	inline app([[maybe_unused]] int argc, [[maybe_unused]] char** argv) noexcept
	{
	}

public:
	int exec() noexcept;

private:
	void cmd_help(std::stringstream& parameters);
	void cmd_pixel(std::stringstream& parameters);
	void cmd_canvas(std::stringstream& parameters);
	void cmd_output(std::stringstream& parameters);

private:
	void check_canvas();

private:
	struct cmd_entry;
	static const cmd_entry* commands() noexcept;
	static const cmd_entry* look_for_command(const char* cmd) noexcept;

private:
	std::unique_ptr<canvas> canvas_ {};
};

#endif // APP_H
