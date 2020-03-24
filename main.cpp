
#include "Camera.hpp"

std::string path = "C:\\Users\\s6417\\OneDrive\\Stuff\\Year 4\\4900 Design I\\TheControlApp\\ControlApp\\x64\\CV_Debug\\";
constexpr int delay = 100;


volatile bool CaptureSwitchOn = false;

int control_main()
{
	while (1)
	{

		if (std::cin.get() == 'l')
		{
			std::cout << "Capture..." << std::endl;
			CaptureSwitchOn = true;
		}
	}
	return 0;
}

int main()
{

	std::thread Control(control_main);

	camera a(path + "A\\", 0);
	camera b(path + "B\\", 1);
	camera c(path + "C\\", 2);
	camera d(path + "D\\", 3);

	if (a.startCapture() == false) {
		std::cout << "Camera A false...";
		std::cin.get();
	}
	if (b.startCapture() == false) {
		std::cout << "Camera B false...";
		std::cin.get();
	}
	if (c.startCapture() == false) {
		std::cout << "Camera C false...";
		std::cin.get();
	}
	if (d.startCapture() == false) {
		std::cout << "Camera D false...";
		std::cin.get();
	}
	

	//for (int i = 0; i < 100; i++)
	//{
	//	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	//	std::cout << "Storing a frame...." << std::endl;
	//	a.storeImage(i);
	//	b.storeImage(i);
	//	c.storeImage(i);
	//	d.storeImage(i);
	//}

	int i = 0;

	while (1)
	{
		if (CaptureSwitchOn == true)
		{
			CaptureSwitchOn = false;
			std::cout << "Storing a frame...." << std::endl;
			a.storeImage(i);
			b.storeImage(i);
			c.storeImage(i);
			d.storeImage(i);
			i++;
		}

	}

	a.stopCapture();
	a.~camera();

	b.stopCapture();
	b.~camera();

	c.stopCapture();
	c.~camera();

	d.stopCapture();
	d.~camera();

	std::cin.get();
	return 0;
}