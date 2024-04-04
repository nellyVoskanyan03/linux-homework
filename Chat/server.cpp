#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 1024

int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);
    }

    sockaddr_in server_address;
    
    memset(&server_address, 0, sizeof(sockaddr_in));

    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(4097);

    int bound = bind(sock_fd, (sockaddr*)&server_address, sizeof(server_address));
    if(bound < 0)
    {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);
    }

    int listened = listen(sock_fd, 5);
    if(listened < 0)
    {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);       
    }

    while(true)
    {
        char buffer[BUF_SIZE + 1];

        int client_fd = accept(sock_fd, NULL, NULL);
        if(client_fd < 0)
        {
            std::cerr << strerror(errno) << std::endl;
            exit(errno);
        }
        int message_size;
        int bytes_read = read(client_fd, &message_size, sizeof(int));
        if(bytes_read < 0)
        {
            std::cerr << strerror(errno) << std::endl;
            exit(errno);
        }
        message_size = ntohl(message_size);

        while(message_size > 0)
        {
            bytes_read = read(client_fd, buffer, std::min(BUF_SIZE, message_size));
            if(bytes_read < 0)
            {
                std::cerr << strerror(errno) << std::endl;
                exit(errno);
            }
            
            message_size -= bytes_read;
                
            buffer[bytes_read] = '\0';
            std::cout << buffer;
        }
        std::cout << std::endl;

        close(client_fd);
    }

    close(sock_fd);

    return 0;

}
