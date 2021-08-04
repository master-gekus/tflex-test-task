#ifndef APP_H
#define APP_H

#include <sstream>

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

private:
	struct cmd_entry;
	static const cmd_entry* commands() noexcept;
	static const cmd_entry* look_for_command(const char *cmd) noexcept;
};

#endif // APP_H
