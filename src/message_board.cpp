//
// Created by 310165137 on 14/01/2021.
//
#include <algorithm>
#include <vector>
#include "topic.h"
#include "message_board.h"

std::mutex message_board::mutex_msgboard;

bool message_board::isTopicExist(std::string topic) {
#if 0
    auto copy_mess_board = _message_board;
    auto it = copy_mess_board.begin();
    unsigned int counter = 0;
    const auto size = copy_mess_board.size();
    while (it != copy_mess_board.end())
    {
        if ((*it)->getTopic() == topic) {
            return true;
        }

        if (counter > size)
            throw 1;

        ++it;
        counter++;
    }
#else
    for (auto item: _message_board) {
        if (item->getTopic() == topic) {
            return true;
        }
    }
#endif
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
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
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
    if (this->isTopicExist(usertopic))
    {
        topic *curr_topic = this->getTopic(usertopic);
        postId = curr_topic->getPosts().size();
        curr_topic->insertPost(postId, message);
    }
    else
    {
        this->createNewTopic(usertopic);
        topic *curr_topic = this->getTopic(usertopic);
        postId = curr_topic->getPosts().size();
        curr_topic->insertPost(postId, message);
    }
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
    std::string post("");
    if (this->isTopicExist(userTopic)) {
        topic *curr_topic = this->getTopic(userTopic);
        if (curr_topic != nullptr &&  curr_topic->getPosts().size() > i) {
            post = curr_topic->getPosts()[i]->getMessage();
        }
    }
    return post;
}

const std::vector<topic *> &message_board::getMessageBoard() const {
    return _message_board;
}
