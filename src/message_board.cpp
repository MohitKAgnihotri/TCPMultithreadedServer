//
// Created by 310165137 on 14/01/2021.
//
#include <algorithm>
#include "topic.h"
#include "message_board.h"

bool message_board::isTopicExist(std::string topic) {
    if (std::find(getMessageBoard().cbegin(),getMessageBoard().cend(), topic) != getMessageBoard().cend())
        return true;
    else
        return false;
}

topic message_board::getTopic(std::string topic) {
    auto it = std::find(getMessageBoard().cbegin(), getMessageBoard().cend(), topic);
    if (it != getMessageBoard().cend())
    {
        return *it;
    }
}

void message_board::createNewTopic(std::string topicStr) {
    topic pTopic = topic(topicStr);
    _message_board.push_back(pTopic);
}

bool message_board::operator==(const message_board &rhs) const {
    return _message_board == rhs._message_board;
}

bool message_board::operator!=(const message_board &rhs) const {
    return !(rhs == *this);
}
