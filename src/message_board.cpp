//
// Created by 310165137 on 14/01/2021.
//
#include <algorithm>
#include <vector>
#include "topic.h"
#include "message_board.h"

bool message_board::isTopicExist(std::string topic) {
    for (auto item: this->getMessageBoard()) {
        if (item->getTopic() == topic) {
            return true;
        }
    }
    return false;
}

topic *message_board::getTopic(std::string usertopic) {
    for (auto item:this->getMessageBoard()) {
        if (item->getTopic() == usertopic) {
            return item;
        }
    }
    return nullptr;
}

void message_board::createNewTopic(std::string topicStr) {
    topic *pTopic = new topic(topicStr);
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
    mutex.lock();
    if (this->isTopicExist(usertopic)) {
        topic *curr_topic = this->getTopic(usertopic);
        postId = curr_topic->getPosts().size();
        curr_topic->insertPost(postId, message);
    } else {
        this->createNewTopic(usertopic);
        topic *curr_topic = this->getTopic(usertopic);
        postId = curr_topic->getPosts().size();
        curr_topic->insertPost(postId, message);
    }
    mutex.unlock();
    return postId;
}

unsigned int message_board::Count(std::string usertopic) {
    if (this->isTopicExist(usertopic)) {
        return this->getTopic(usertopic)->getPosts().size();
    } else {
        return 0;
    }
}

std::string message_board::List() {
    std::string topic_list;
    for (auto item:this->getMessageBoard()) {
        topic_list.append(item->getTopic());
        topic_list.append("#");
    }
    return topic_list;

}

std::string message_board::Read(std::string userTopic, unsigned int i) {
    std::string post("          ");
    if (this->isTopicExist(userTopic)) {
        topic *curr_topic = this->getTopic(userTopic);
        if (curr_topic->getPosts().size() > i) {
            post = curr_topic->getPosts()[i]->getMessage();
        }
    }
    return post;
}

const std::vector<topic *> &message_board::getMessageBoard() const {
    return _message_board;
}
