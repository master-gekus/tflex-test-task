#include "output_device_console_win.h"

#include <Windows.h>

#include "color.h"

output_device_factory::registrar output_device_console_win::registrar_ {
	"console", []([[maybe_unused]] std::stringstream& paramters) {
		return new output_device_console_win {};
	}};

class output_device_console_win::data final
{
public:
	inline data()
		: h_console_ {::GetStdHandle(STD_OUTPUT_HANDLE)}
		, saved_console_attr_ {FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE}
	{
	}

	inline void save_console_attr()
	{
		CONSOLE_SCREEN_BUFFER_INFO sbi;
		::GetConsoleScreenBufferInfo(h_console_, &sbi);
		saved_console_attr_ = sbi.wAttributes;
	}

	inline void restore_console_attr()
	{
		::SetConsoleTextAttribute(h_console_, saved_console_attr_);
	}

	inline void set_console_attr(WORD attr)
	{
		if (last_used_console_attr_ != attr)
		{
			::SetConsoleTextAttribute(h_console_, attr);
			last_used_console_attr_ = attr;
		}
	}

private:
	HANDLE h_console_;
	WORD saved_console_attr_;
	WORD last_used_console_attr_ {static_cast<WORD>(-1)};
};

output_device_console_win::output_device_console_win()
	: data_ {new data {}}
{
}

output_device_console_win::~output_device_console_win()
{
}

void output_device_console_win::save_console_attr()
{
	data_->save_console_attr();
}

void output_device_console_win::restore_console_attr()
{
	data_->restore_console_attr();
}

void output_device_console_win::set_console_attr(color c)
{
	const uint8_t r {c.red()};
	const uint8_t g {c.green()};
	const uint8_t b {c.blue()};

	const bool ri {r > 128};
	const bool gi {g > 128};
	const bool bi {b > 128};

	const bool is_intensified {ri || gi || bi};
	uint8_t limit {static_cast<uint8_t>(is_intensified ? 192 : 64)};

	WORD attr {0};
	if (r > limit)
	{
		attr |= FOREGROUND_RED;
	}

	if (g > limit)
	{
		attr |= FOREGROUND_GREEN;
	}

	if (b > limit)
	{
		attr |= FOREGROUND_BLUE;
	}

	if (is_intensified)
	{
		attr |= FOREGROUND_INTENSITY;
	}

	static constexpr auto silver {static_cast<uint8_t>(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)};
	static constexpr auto gray {static_cast<uint8_t>(FOREGROUND_INTENSITY)};

	if (silver == attr)
	{
		attr = gray;
	}
	else if (gray == attr)
	{
		attr = silver;
	}

	data_->set_console_attr(attr);
}
