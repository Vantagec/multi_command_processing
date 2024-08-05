#include <iostream>
#include <sstream>
#include <thread>
#include <utility>
#include <async.h>
#include <streamredirect.hpp>
#include "version.h"
//01
// using namespace std;

void printUsage(const char *argv0) {
    std::cout << "Usage:" << std::endl;
    std::cout << argv0 << " [number of commands]" << std::endl;
}

void processCommands(int bulkCommandsLimit) {
    std::string command;
    auto handle = async::connect(static_cast<std::size_t>(bulkCommandsLimit));

    while (std::cin >> command) {
        async::receive(handle, command.c_str(), command.size());
    }

    async::disconnect(handle);
}

void runExample1(int bulkCommandsLimit) {
    std::cout << std::endl << "example1: " << std::endl;

    std::istringstream input("cmd1\ncmd2\ncmd3\ncmd4\ncmd5\nEOF");
    helpers::stream_redirect redirect {std::cin, input.rdbuf()};

    processCommands(bulkCommandsLimit);
}

void runExample2(int bulkCommandsLimit) {
    std::cout << std::endl << "example2: " << std::endl;

    std::stringstream input;
    input << "cmd1\n"
          << "cmd2\n"
          << "{\n"
          << "cmd3\n"
          << "cmd4\n"
          << "}\n"
          << "{\n"
          << "cmd5\n"
          << "cmd6\n"
          << "{\n"
          << "cmd7\n"
          << "cmd8\n"
          << "}\n"
          << "cmd9\n"
          << "}\n"
          << "{\n"
          << "cmd10\n"
          << "cmd11\n"
          << "EOF\n";

    helpers::stream_redirect redirect {std::cin, input.rdbuf()};

    processCommands(bulkCommandsLimit);
}

int main(int argc, const char *argv[]) {
    std::cout << "Async app version: " << PROJECT_VERSION << std::endl;

    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    int bulkCommandsLimit = std::atoi(argv[1]);
    if (bulkCommandsLimit < 1) return 0;

    runExample1(bulkCommandsLimit);
    runExample2(bulkCommandsLimit);

    return 0;
}
