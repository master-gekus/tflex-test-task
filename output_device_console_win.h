#ifndef OUTPUT_DEVICE_CONSOLE_WIN_H
#define OUTPUT_DEVICE_CONSOLE_WIN_H

#include <memory>

#include "output_device_console.h"
#include "output_device_factory.h"

class output_device_console_win : public output_device_console
{
public:
	output_device_console_win();
	~output_device_console_win() override;

protected:
	void save_console_attr() override;
	void restore_console_attr() override;
	void set_console_attr(color c) override;

private:
	class data;
	std::unique_ptr<data> data_;

private:
	static output_device_factory::registrar registrar_;
};

#endif // OUTPUT_DEVICE_CONSOLE_WIN_H
