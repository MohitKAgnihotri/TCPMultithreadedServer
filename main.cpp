
#include <iostream>
#include <string>
#include <vector>

#include "RequestParser.h"
#include "post.h"
#include "message_board.h"

#include <iostream>
#include <csignal>
#include <string>

#include "include/tcp_server.h"

//#define DEBUG 0

// declare the server
TcpServer server;

// declare the message board
message_board my_message_board;

// declare a server observer which will receive incoming messages.
// the server supports multiple observers
server_observer_t message_board_observer;

// observer callback. will be called for every new message received by clients
// with the requested IP address
void process_incoming_message(const Client *client, const char *msg, size_t size) {
    std::string msgStr = msg;
    // print the message content
    // if client sent the string "quit", close server
    // else if client sent "print" print the server clients
    // else just print the client message
    if (msgStr.find("POST") != std::string::npos) {
#ifdef DEBUG
        std::cout << "Post Request Received" << std::endl;
#endif
        PostRequest post = PostRequest::parse(msgStr);
        if (post.valid) {
#ifdef DEBUG
            std::cout << "Post request: " << post.toString() << std::endl;
            std::cout << "Post topic: " << post.getTopicId() << std::endl;
            std::cout << "Post message: " << post.getMessage() << std::endl;
#endif
            unsigned int postId = my_message_board.Post(post.getTopicId(), post.getMessage());
            server.sendToClient(client, std::to_string(postId).c_str(), std::to_string(postId).length());
        }
    } else if (msgStr.find("LIST") != std::string::npos) {
#ifdef DEBUG
        std::cout << "List Request Received" << std::endl;
#endif
        ListRequest list = ListRequest::parse(msgStr);
        if (list.valid) {
#ifdef DEBUG
            std::cout << "List request: " << list.toString() << std::endl;
#endif
            std::string topic_string = my_message_board.List();
            server.sendToClient(client, topic_string.c_str(), topic_string.length());
        }
    } else if (msgStr.find("COUNT") != std::string::npos) {
#ifdef DEBUG
        std::cout << "COUNT Request Received" << std::endl;
#endif
        CountRequest count = CountRequest::parse(msgStr);
        if (count.valid) {
#ifdef DEBUG
            std::cout << "Count request: " << count.toString() << std::endl;
            std::cout << "Count topic: " << count.getTopicId() << std::endl;
#endif
            unsigned int num_of_post = my_message_board.Count(count.getTopicId());
            server.sendToClient(client, std::to_string(num_of_post).c_str(), std::to_string(num_of_post).length());
        }
    } else if (msgStr.find("READ") != std::string::npos) {
#ifdef DEBUG
        std::cout << "READ Request Received" << std::endl;
#endif
        ReadRequest read = ReadRequest::parse(msgStr);
        if (read.valid) {
#ifdef DEBUG
            std::cout << "Read request" << read.toString() << std::endl;
            std::cout << "Read topic: " << read.getTopicId() << std::endl;
            std::cout << "Read post id: " << read.getPostId() << std::endl;
#endif
            std::string post_message = my_message_board.Read(read.getTopicId(), read.getPostId());
            if (post_message.length() != 0)
            {
                pipe_ret_t ret = server.sendToClient(client, post_message.c_str(), post_message.length());
                if (!ret.success)
                {
                    std::cout << "failed to send message to the client" << ret.msg << client->getFileDescriptor()
                              << std::endl;
                    server.sendToClient(client, post_message.c_str(), post_message.length());
                }
            }
            else
            {
                std::string failed_response("No such post found");
                pipe_ret_t ret = server.sendToClient(client, failed_response.c_str(), failed_response.length());
                if (!ret.success) {
                    std::cout << "failed to send message to the client" << ret.msg << client->getFileDescriptor()
                              << std::endl;
                    server.sendToClient(client, failed_response.c_str(), failed_response.length());
                }
            }
        }
    } else if (msgStr.find("EXIT") != std::string::npos) {
#ifdef DEBUG
        std::cout << "EXIT Request Received" << std::endl;
#endif
        ExitRequest exitReq = ExitRequest::parse(msgStr);
        if (exitReq.valid) {
#ifdef DEBUG
            std::cout << "Exit request: " << exitReq.toString() << std::endl;
#endif
#if 0
            for (auto item_topic:my_message_board.getMessageBoard())
            {
                for (auto item : item_topic->getPosts())
                {
                    free(item);
                }
                free(item_topic);
            }
            pipe_ret_t ret = server.finish();
            if (ret.success)
            {
                server.sendToClient(client,msgStr.c_str(), msgStr.length());
                kill(getppid(), SIGEV_SIGNAL);
            }
#endif
            server.sendToClient(client, msgStr.c_str(), msgStr.length());
        }
    } else {
        std::cout << "Invalid Request" << msgStr << std::endl;
        server.sendToClient(client, msgStr.c_str(), msgStr.length());
    }
}

// observer callback. will be called when client disconnects
void onClientDisconnected(const Client *client) {
    std::cout << "Client: " << client->getIp() << " disconnected: " << client->getInfoMessage() << std::endl;
}

void signal_user1_handler(int signal) {
    std::cout << "Handling User Signal 1" << std::endl;
    for (auto item_topic:my_message_board.getMessageBoard()) {
        for (auto item : item_topic->getPosts()) {
            free(item);
        }
        free(item_topic);
    }

    exit(0);
}

int main() {

    // start server on port 65123
    pipe_ret_t startRet = server.start(12345);
    if (startRet.success) {
        std::cout << "Server setup succeeded" << std::endl;
    } else {
        std::cout << "Server setup failed: " << startRet.msg << std::endl;
        return EXIT_FAILURE;
    }

    // configure and register observer1
    message_board_observer.incoming_packet_func = process_incoming_message;
    message_board_observer.disconnected_func = onClientDisconnected;
    message_board_observer.wantedIp = "";
    server.subscribe(message_board_observer);

    // receive clients
    while (1) {
        Client *client = server.acceptClient();
        if (client->isConnected()) {
            std::cout << "Got client with IP: " << client->getIp() << std::endl;
            server.printClients();
        } else {
            std::cout << "Accepting client failed: " << client->getInfoMessage() << std::endl;
        }
    }
}
