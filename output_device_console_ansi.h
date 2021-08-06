#ifndef OUTPUT_DEVICE_CONSOLE_ANSI_H
#define OUTPUT_DEVICE_CONSOLE_ANSI_H

#include <memory>

#include "output_device_console.h"
#include "output_device_factory.h"

class output_device_console_ansi : public output_device_console
{
public:
	output_device_console_ansi() = default;
	~output_device_console_ansi() override = default;

protected:
	void save_console_attr() override;
	void restore_console_attr() override;
	void set_console_attr(color c) override;

private:
	uint8_t last_used_attr_ {static_cast<uint8_t>(-1)};

private:
	static output_device_factory::registrar registrar_;
};

#endif // OUTPUT_DEVICE_CONSOLE_ANSI_H
