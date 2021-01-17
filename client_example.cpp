///////////////////////////////////////////////////////////
/////////////////////CLIENT EXAMPLE////////////////////////
///////////////////////////////////////////////////////////

#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <errno.h>
#include <thread>
#include <cstring>
#include<chrono>
#include "pipe_ret_t.h"

#define MAX_PACKET_SIZE 4096

std::vector<std::string> post_requestList =
        {
                "POST@Solar system#The dark side of the @moon is #dark.",
                "POST@#x",
                "POST@night_sky#cloudy!",
                "POST@night_sky#clear!",
                "POST@Solar system#Mercury, Venus, Earth, Mars, Jupiter, Saturn, Neptune, Uranus",
        };

std::vector<std::string> read_requestList =
        {
                "READ@#0",
                "READ@Solar system#1",
                "READ@night_sky#0",
                "READ@night_sky#2",
                "READ@solar_system#0",
        };


std::string server_ip;
int port_num;
int num_of_post_thread;
int num_of_read_thread;
int time_to_test;

void send_post_req(void) {
    int sockfd = 0;
    struct sockaddr_in server;
    pipe_ret_t ret;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { //socket failed
        ret.success = false;
        ret.msg = strerror(errno);
        std::cout << ret.msg << std::endl;
        exit(0);
    }

    int inetSuccess = inet_aton(server_ip.c_str(), &server.sin_addr);

    if (!inetSuccess) { // inet_addr failed to parse address
        // if hostname is not in IP strings and dots format, try resolve it
        struct hostent *host;
        struct in_addr **addrList;
        if ((host = gethostbyname(server_ip.c_str())) == NULL) {
            ret.success = false;
            ret.msg = "Failed to resolve hostname";
            std::cout << ret.msg << std::endl;
            exit(0);
        }
        addrList = (struct in_addr **) host->h_addr_list;
        server.sin_addr = *addrList[0];
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port_num);

    int connectRet = connect(sockfd, (struct sockaddr *) &server, sizeof(server));
    if (connectRet == -1) {
        ret.success = false;
        ret.msg = strerror(errno);
        std::cout << ret.msg << std::endl;
        exit(0);
    }

    int message_cnt = 0;
    auto start = std::chrono::system_clock::now();
    while ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() <=
            time_to_test)) {
        for (auto item:read_requestList) {
            pipe_ret_t ret;
            int numBytesSent = send(sockfd, item.c_str(), item.size(), 0);
            if (numBytesSent < 0) { // send failed
                ret.success = false;
                ret.msg = strerror(errno);
                exit(0);
            }

            if ((uint) numBytesSent < item.size()) { // not all bytes were sent
                ret.success = false;
                char msg[100];
                sprintf(msg, "Only %d bytes out of %lu was sent to client", numBytesSent, item.size());
                ret.msg = msg;
                exit(0);
            }

            char msg[MAX_PACKET_SIZE];
            memset(msg, 0x00, sizeof(char) * MAX_PACKET_SIZE);
            int numOfBytesReceived = recv(sockfd, msg, MAX_PACKET_SIZE, 0);
            if (numOfBytesReceived < 1) {
                pipe_ret_t ret;
                ret.success = false;
                if (numOfBytesReceived == 0) { //server closed connection
                    ret.msg = "Server closed connection";
                    exit(0);
                } else {
                    ret.msg = strerror(errno);
                    std::cout << ret.msg << std::endl;
                }
            } else {
                std::cout << msg << std::endl;
            }
            message_cnt++;

        }
    }
    std::cout << "ThreadId = " << std::this_thread::get_id() << "Sent " << message_cnt << " in "
              << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count()
              << "seconds" << std::endl;
}

void send_read_req(void) {
    int sockfd = 0;
    struct sockaddr_in server;
    pipe_ret_t ret;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { //socket failed
        ret.success = false;
        ret.msg = strerror(errno);
        std::cout << ret.msg << std::endl;
        exit(0);
    }

    int inetSuccess = inet_aton(server_ip.c_str(), &server.sin_addr);

    if (!inetSuccess) { // inet_addr failed to parse address
        // if hostname is not in IP strings and dots format, try resolve it
        struct hostent *host;
        struct in_addr **addrList;
        if ((host = gethostbyname(server_ip.c_str())) == NULL) {
            ret.success = false;
            ret.msg = "Failed to resolve hostname";
            std::cout << ret.msg << std::endl;
            exit(0);
        }
        addrList = (struct in_addr **) host->h_addr_list;
        server.sin_addr = *addrList[0];
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(port_num);

    int connectRet = connect(sockfd, (struct sockaddr *) &server, sizeof(server));
    if (connectRet == -1) {
        ret.success = false;
        ret.msg = strerror(errno);
        std::cout << ret.msg << std::endl;
        exit(0);
    }

    int message_cnt = 0;
    auto start = std::chrono::system_clock::now();
    while ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() <=
            time_to_test)) {
        for (auto item:post_requestList) {
            pipe_ret_t ret;
            int numBytesSent = send(sockfd, item.c_str(), item.size(), 0);
            if (numBytesSent < 0) { // send failed
                ret.success = false;
                ret.msg = strerror(errno);
                exit(0);
            }

            if ((uint) numBytesSent < item.size()) { // not all bytes were sent
                ret.success = false;
                char msg[100];
                sprintf(msg, "Only %d bytes out of %lu was sent to client", numBytesSent, item.size());
                ret.msg = msg;
                exit(0);
            }

            char msg[MAX_PACKET_SIZE];
            memset(msg, 0x00, sizeof(char) * MAX_PACKET_SIZE);
            int numOfBytesReceived = recv(sockfd, msg, MAX_PACKET_SIZE, 0);
            if (numOfBytesReceived < 1) {
                pipe_ret_t ret;
                ret.success = false;
                if (numOfBytesReceived == 0) { //server closed connection
                    ret.msg = "Server closed connection";
                    exit(0);
                } else {
                    ret.msg = strerror(errno);
                    std::cout << ret.msg << std::endl;
                }
            } else {
                std::cout << msg << std::endl;
            }
            message_cnt++;
        }
    }
    std::cout << "ThreadId = " << std::this_thread::get_id() << "Sent " << message_cnt << " in "
              << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count()
              << "seconds" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 6) {
        std::cout << "Usage:" << argv[0] << " server_ip port_num num_post_thread num_read_thread test_duration(sec)"
                  << std::endl;
        exit(0);
    }

    server_ip = std::string(argv[1]);
    port_num = std::stoi(argv[2]);
    num_of_post_thread = std::stoi(argv[3]);
    num_of_read_thread = std::stoi(argv[4]);
    time_to_test = std::stoi(argv[5]);

    std::vector<std::thread> post_req_threads;
    std::vector<std::thread> read_req_threads;

    for (int i = 0; i < num_of_post_thread; i++) {
        post_req_threads.push_back(std::move(std::thread(&send_post_req)));

    }

    for (int i = 0; i < num_of_read_thread; i++) {
        read_req_threads.push_back(std::move(std::thread(&send_read_req)));
    }

    for (int i = 0; i < post_req_threads.size(); i++)
        post_req_threads[i].join();
    for (int i = 0; i < read_req_threads.size(); i++)
        read_req_threads[i].join();
}