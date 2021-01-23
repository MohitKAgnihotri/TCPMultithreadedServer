
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
#include <random>
#include <mutex>
#include "pipe_ret_t.h"

#define MAX_PACKET_SIZE 4096

std::string server_ip;
int port_num;
int num_of_post_thread;
int num_of_read_thread;
int time_to_test;
int topic_message_len;

std::mutex mutex_thread_print;

int ConnectToServer();

void SendReqToServer(int sockfd, const std::string &post_request);

/* Function to create strings of given length
 * */
std::string random_string(std::string::size_type length)
{
    // Constant look-up table
    static auto& chrs = "0123456789"
                        "abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;
    s.reserve(length);

    while(length--)
        s += chrs[pick(rg)];

    return s;
}


std:: string GetPostRequest(std::string::size_type length)
{
    return std::string("POST@") + random_string(length) + std::string("#") + random_string(length);
}

std:: string GetReadRequest(std::string::size_type length, unsigned int post_index)
{
    return std::string("READ@") + random_string(length) + std::string("#") + std::to_string(post_index);
}

int ConnectToServer()
{
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
    return sockfd;
}

void SendReqToServer(int sockfd, const std::string &post_request) {
    pipe_ret_t ret;
    int numBytesSent = send(sockfd, post_request.c_str(), post_request.size(), 0);
    if (numBytesSent < 0) { // send failed
        ret.success = false;
        ret.msg = strerror(errno);
        exit(0);
    }

    if ((uint) numBytesSent < post_request.size())
    { // not all bytes were sent
        ret.success = false;
        char msg[100];
        sprintf(msg, "Only %d bytes out of %lu was sent to client", numBytesSent, post_request.size());
        ret.msg = msg;
        exit(0);
    }

    char msg[MAX_PACKET_SIZE];
    memset(msg, 0x00, sizeof(char) * MAX_PACKET_SIZE);
    int numOfBytesReceived = recv(sockfd, msg, MAX_PACKET_SIZE, 0);
    if (numOfBytesReceived < 1)
    {
        pipe_ret_t ret;
        ret.success = false;
        if (numOfBytesReceived == 0)
        { //server closed connection
            ret.msg = "Server closed connection";
            exit(0);
        }
        else
        {
            ret.msg = strerror(errno);
            std::cout << ret.msg << std::endl;
        }
    }
    else
    {
        //std::cout << msg << std::endl;
    }
}

void send_post_req(void)
{
    int sockfd = ConnectToServer();

    int message_cnt = 0;
    auto start = std::chrono::system_clock::now();
    while ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() <=
            time_to_test))
    {
        std::string post_request = GetPostRequest (topic_message_len);
        SendReqToServer(sockfd, post_request);
        message_cnt++;
    }

    mutex_thread_print.lock();
    std::cout << "Post ThreadId = " << std::this_thread::get_id() << " Sent " << message_cnt << " in "
              << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count()
              << "seconds " << std::endl;
    mutex_thread_print.unlock();
    close(sockfd);
}

void send_read_req(void) {

    int sockfd = ConnectToServer();

    int message_cnt = 0;
    auto start = std::chrono::system_clock::now();
    while ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() <=
            time_to_test))
    {
        std::string post_request = GetReadRequest (topic_message_len, std::rand()%100);
        SendReqToServer(sockfd, post_request);
        message_cnt++;
    }
    mutex_thread_print.lock();
    std::cout << "Read ThreadId = " << std::this_thread::get_id() << " Sent " << message_cnt << " in "
              << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count()
              << "seconds " << std::endl;
    mutex_thread_print.unlock();
    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        std::cout << "Usage:" << argv[0] << " server_ip port_num num_post_thread num_read_thread test_duration(sec) request_Length"
                  << std::endl;
        exit(0);
    }

    server_ip = std::string(argv[1]);
    port_num = std::stoi(argv[2]);
    num_of_post_thread = std::stoi(argv[3]);
    num_of_read_thread = std::stoi(argv[4]);
    time_to_test = std::stoi(argv[5]);
    topic_message_len = std::stoi(argv[6]);

    std::vector<std::thread> post_req_threads;
    std::vector<std::thread> read_req_threads;

    for (int i = 0; i < num_of_post_thread; i++) {
        post_req_threads.push_back(std::thread(&send_post_req));
    }

    for (int i = 0; i < num_of_read_thread; i++) {
        read_req_threads.push_back(std::thread(&send_read_req));
    }

    for (long unsigned int i = 0; i < post_req_threads.size(); i++)
        post_req_threads[i].join();
    for (long unsigned int i = 0; i < read_req_threads.size(); i++)
        read_req_threads[i].join();
}
