#ifndef OUTPUT_DEVICE_CONSOLE_H
#define OUTPUT_DEVICE_CONSOLE_H

#include "output_device_interface.h"

class color;

class output_device_console : public output_device_interface
{
public:
	output_device_console() = default;
	~output_device_console() override = default;

protected:
	void output_canvas(const canvas*) override;

	virtual void save_console_attr() = 0;
	virtual void restore_console_attr() = 0;
	virtual void set_console_attr(color c) = 0;
};

#endif // OUTPUT_DEVICE_CONSOLE_H
