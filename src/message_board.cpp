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

unsigned int message_board::Post(std::string usertopic, std::string message) {
    unsigned int postId = 0;
    if (this->isTopicExist(usertopic)) {
        topic curr_topic = this->getTopic(usertopic);
        postId = curr_topic.getPosts().size();
        curr_topic.insertPost(postId, message);
    } else {
        this->createNewTopic(usertopic);
        topic curr_topic = this->getTopic(usertopic);
        postId = curr_topic.getPosts().size();
        curr_topic.insertPost(postId, message);
    }
    return postId;
}

unsigned int message_board::Count(std::string usertopic) {
    if (this->isTopicExist(usertopic)) {
        return this->getTopic(usertopic).getPosts().size();
    } else {
        return 0;
    }
}

std::string message_board::List() {
    std::string topic_list;
    for (auto item:this->getMessageBoard()) {
        topic_list.append(item.getTopic());
        topic_list.append("#");
    }
    return topic_list;

}

std::string message_board::Read(std::string basicString, int i) {
    return std::string();
}
