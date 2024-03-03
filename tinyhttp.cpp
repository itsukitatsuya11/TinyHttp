#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>

class HttpServer {
public:
    HttpServer(int port) : port_(port) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            root_directory_ = std::string(cwd);
        } else {
            std::cerr << "Error getting current directory" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void start() {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            std::cerr << "Socket creation failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port_);

        if (bind(server_fd, (sockaddr *)&address, sizeof(address)) < 0) {
            std::cerr << "Bind failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0) {
            std::cerr << "Listen failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "TinyHttp is running on port " << port_ << std::endl;
        std::cout << "URL: http://localhost:" << port_ << std::endl;

        while (true) {
            int client_socket = accept(server_fd, nullptr, nullptr);
            if (client_socket < 0) {
                std::cerr << "Accept failed" << std::endl;
                exit(EXIT_FAILURE);
            }

            handle_request(client_socket);
            close(client_socket);
        }
    }

private:
    int port_;
    std::string root_directory_;

    void handle_request(int client_socket) {
        char buffer[4096] = {0};
        read(client_socket, buffer, sizeof(buffer));

        std::istringstream request(buffer);
        std::string method, path, protocol;
        request >> method >> path >> protocol;

        if (path == "/") {
            path = "/index.html";
        }

        std::string full_path = root_directory_ + path;

        std::ifstream file(full_path, std::ios::binary);
        if (file) {
            std::string response = "HTTP/1.1 200 OK\r\nContent-Length: ";
            file.seekg(0, std::ios::end);
            size_t file_size = file.tellg();
            file.seekg(0, std::ios::beg);
            response += std::to_string(file_size);
            response += "\r\n\r\n";
            send(client_socket, response.c_str(), response.length(), 0);

            char file_buffer[1024];
            while (file.read(file_buffer, sizeof(file_buffer)).gcount() > 0) {
                send(client_socket, file_buffer, file.gcount(), 0);
            }
        } else {
            std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
            send(client_socket, response.c_str(), response.length(), 0);
        }
    }
};

int main(int argc, char *argv[]) {
    int port = 8000; // Port default

    if (argc == 3 && std::string(argv[1]) == "-port") {
        port = std::stoi(argv[2]);
    } else {
        std::cerr << "Usage: " << argv[0] << " -port [port_number]" << std::endl;
        return 1;
    }

    HttpServer server(port);
    server.start();
    return 0;
}
