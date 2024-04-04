#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        std::cerr << "Argument error" << std::endl;
        exit(1);
    }

    char* s_ip = argv[1];
    char* s_port = argv[2];
    std::string client_name = "client2: ";
    while(true){
        int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(sock_fd < 0)
        {
            std::cerr << strerror(errno) << std::endl;
            exit(errno);
        }

        sockaddr_in server_address;
        
        memset(&server_address, 0, sizeof(sockaddr_in));

        inet_pton(AF_INET, s_ip, &(server_address.sin_addr));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(std::stoi(s_port));

        int connected = connect(sock_fd, (sockaddr*) &server_address, sizeof(server_address));
        if(connected < 0)
        { 
            std::cerr << strerror(errno) << std::endl;
            exit(errno);       
        }
        
        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        message = client_name + message;
        std::cout << "Sending " << message.size() << " bytes to server" << std::endl;

        int message_size = htonl(message.size());
        write(sock_fd, &message_size, sizeof(message_size));
        write(sock_fd, message.c_str(), message.size());

        close(sock_fd);
    }
   

    return 0;
}
 