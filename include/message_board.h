
#ifndef TCP_CLIENT_SERVER_MESSAGE_BOARD_H
#define TCP_CLIENT_SERVER_MESSAGE_BOARD_H

#include "topic.h"
#include <unordered_map>

class message_board {
    std::unordered_map<std::string, std::vector<std::string>> _message_board_map;
    static std::mutex mutex_msgboard;
public:

    std::string Read(std::string userTopic, unsigned int i);

    std::string List();

    unsigned int Count(std::string usertopic);

    unsigned int Post(std::string usertopic, std::string message);

    void createNewPost(std::string topicStr, std::string message);

    void createNewTopic(std::string topicStr);

    bool isTopicExist(std::string topic);
};


#endif //TCP_CLIENT_SERVER_MESSAGE_BOARD_H
