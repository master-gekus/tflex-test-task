#include <iostream>
#include <filesystem>
#include <thread>

int main()
{
	std::cout << "Starting thread..." << std::endl;

	std::thread([]() {
		auto cwd {std::filesystem::current_path()};
		std::cout << "Current path is " << cwd << std::endl;

		auto parent {cwd.parent_path()};
		std::cout << "Parent path is " << parent << std::endl;

		std::cout << "List of CWD:" << std::endl;
		for (auto& p : std::filesystem::recursive_directory_iterator(cwd))
		{
			std::cout << "  - " << p.path() << std::endl;
		}
	}).join();

	std::cout << "Thread finished..." << std::endl;

	return 0;
}
