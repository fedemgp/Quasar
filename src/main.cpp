/**
 * Created by Federico Manuel Gomez Peter 
 * on 20/12/20.
 */
#include "Decoder.h"
#include "Server.h"

int main(int argc, char const *argv[]) {
    try {
        Server server;
        std::cout << "Server initialized. Listening endpoint: " << server.getEndpoint() << std::endl;
        char quit = '\0';
        while (quit != 'q') {
            std::cin >> quit;
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unkown error in main thread" << std::endl;
        return 1;
    }

	return 0;
}