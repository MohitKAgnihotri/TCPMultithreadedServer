//
// Created by 310165137 on 14/01/2021.
//

#ifndef TCP_CLIENT_SERVER_MESSAGE_BOARD_H
#define TCP_CLIENT_SERVER_MESSAGE_BOARD_H

#include "topic.h"

class message_board {
    std::vector<topic*> _message_board;
    static std::mutex mutex_msgboard;
public:
    const std::vector<topic *> &getMessageBoard() const;

public:
    bool isTopicExist(std::string topic);

    topic * getTopic(std::string);

    void createNewTopic(std::string);

    bool operator==(const message_board &rhs) const;

    bool operator!=(const message_board &rhs) const;

    unsigned int Post(std::string basicString, std::string basicString1);

    std::string List();

    unsigned int Count(std::string basicString);

    std::string Read(std::string basicString, unsigned int i);
};


#endif //TCP_CLIENT_SERVER_MESSAGE_BOARD_H
