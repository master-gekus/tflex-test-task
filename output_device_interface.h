#ifndef OUTPUT_DEVICE_INTERFACE_H
#define OUTPUT_DEVICE_INTERFACE_H

class canvas;

class output_device_interface
{
public:
	output_device_interface() = default;
	virtual ~output_device_interface() = default;

public:
	virtual void output_canvas(const canvas*) = 0;
};
#endif // OUTPUT_DEVICE_INTERFACE_H
