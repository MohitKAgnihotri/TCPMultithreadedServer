
#include <iostream>
#include <string>
#include <vector>

#include "RequestParser.h"
#include "post.h"
#include "message_board.h"

#include <iostream>
#include <signal.h>

#include "include/tcp_server.h"

// declare the server
TcpServer server;

// declare the message board
message_board my_message_board;

// declare a server observer which will receive incoming messages.
// the server supports multiple observers
server_observer_t message_board_observer;

// observer callback. will be called for every new message received by clients
// with the requested IP address
void process_incoming_message(const Client &client, const char *msg, size_t size) {
    std::string msgStr = msg;
    // print the message content
    // if client sent the string "quit", close server
    // else if client sent "print" print the server clients
    // else just print the client message
    if (msgStr.find("POST") != std::string::npos) {
        std::cout << "Post Request Received" << std::endl;
        PostRequest post = PostRequest::parse(msgStr);
        if (post.valid) {
            std::cout << "Post request: " << post.toString() << std::endl;
            std::cout << "Post topic: " << post.getTopicId() << std::endl;
            std::cout << "Post message: " << post.getMessage() << std::endl;
            unsigned int postId = my_message_board.Post(post.getTopicId(), post.getMessage());
        }
    } else if (msgStr.find("LIST") != std::string::npos) {
        std::cout << "List Request Received" << std::endl;
        ListRequest list = ListRequest::parse(msgStr);
        if (list.valid) {
            std::cout << "List request: " << list.toString() << std::endl;
            std::string topic_string = my_message_board.List();
        }
    } else if (msgStr.find("COUNT") != std::string::npos) {
        std::cout << "COUNT Request Received" << std::endl;
        CountRequest count = CountRequest::parse(msgStr);
        if (count.valid) {
            std::cout << "Count request: " << count.toString() << std::endl;
            std::cout << "Count topic: " << count.getTopicId() << std::endl;
            unsigned int num_of_post = my_message_board.Count(count.getTopicId());
        }
    } else if (msgStr.find("READ") != std::string::npos) {
        std::cout << "READ Request Received" << std::endl;
        ReadRequest read = ReadRequest::parse(msgStr);
        if (read.valid) {
            std::cout << "Read request" << read.toString() << std::endl;
            std::cout << "Read topic: " << read.getTopicId() << std::endl;
            std::cout << "Read post id: " << read.getPostId() << std::endl;
            std::string post_message = my_message_board.Read(read.getTopicId(), read.getPostId());
        }
    } else if (msgStr.find("EXIT") != std::string::npos) {
        std::cout << "Exit Request Received" << std::endl;
        ExitRequest exitReq = ExitRequest::parse(msgStr);
        if (exitReq.valid) {
            std::cout << "Exit request: " << exitReq.toString() << std::endl;
            server.finish();
        }
    } else {
        std::cout << "Invalid Request" << msgStr << std::endl;
    }
}

// observer callback. will be called when client disconnects
void onClientDisconnected(const Client &client) {
    std::cout << "Client: " << client.getIp() << " disconnected: " << client.getInfoMessage() << std::endl;
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
        Client client = server.acceptClient();
        if (client.isConnected()) {
            std::cout << "Got client with IP: " << client.getIp() << std::endl;
            server.printClients();
        } else {
            std::cout << "Accepting client failed: " << client.getInfoMessage() << std::endl;
        }
    }


    std::vector<std::string> requestList =
            {
                    "POST@Solar system#The dark side of the @moon is #dark.",
                    "POST@#x",
                    "POST@night_sky#cloudy!",
                    "POST@Solar system#Mercury, Venus, Earth, Mars, Jupiter, Saturn, Neptune, Uranus",
                    "LIST",
                    "COUNT@Solar system",
                    "COUNT@solar_system",
                    "READ@#0",
                    "READ@Solar system#1",
                    "READ@night_sky#2",
                    "READ@solar_system#0",
                    "EXIT",
                    "AAA"
            };

    for (std::string &request : requestList)
	{
		PostRequest post = PostRequest::parse(request);
		if (post.valid)
		{
			std::cout << "Post request: " << post.toString() << std::endl;
			std::cout << "Post topic: " << post.getTopicId() << std::endl;
			std::cout << "Post message: " << post.getMessage() << std::endl;
			continue;
		}

		ReadRequest read = ReadRequest::parse(request);
		if (read.valid)
		{
			std::cout << "Read request" << read.toString() << std::endl;
			std::cout << "Read topic: " << read.getTopicId() << std::endl;
			std::cout << "Read post id: " << read.getPostId() << std::endl;
			continue;
		}

		CountRequest count = CountRequest::parse(request);
		if (count.valid)
		{
			std::cout << "Count request: " << count.toString() << std::endl;
			std::cout << "Count topic: " << count.getTopicId() << std::endl;
			continue;
		}

		ListRequest list = ListRequest::parse(request);
		if (list.valid)
		{
			std::cout << "List request: " << list.toString() << std::endl;
			continue;
		}

		ExitRequest exitReq = ExitRequest::parse(request);
		if (exitReq.valid)
		{
			std::cout << "Exit request: " << exitReq.toString() << std::endl;
			continue;
		}

		std::cout << "Unknown request: " << request << std::endl;
		std::cout << std::endl;
	}

	return 0;
}
