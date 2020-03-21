
#include "Camera.hpp"

std::string path = "C:\\Users\\s6417\\OneDrive\\Stuff\\Year 4\\4900 Design I\\TheControlApp\\ControlApp\\x64\\CV_Debug\\";
constexpr int delay = 100;

int main()
{
	camera a(path + "A\\", 0);
	camera b(path + "B\\", 1);
	a.startCapture();
	b.startCapture();

	for (int i = 0; i < 100; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		std::cout << "Storing a frame...." << std::endl;
		a.storeImage(i);
		b.storeImage(i);
	}

	a.stopCapture();
	a.~camera();

	b.stopCapture();
	b.~camera();


	std::cin.get();
	return 0;
}