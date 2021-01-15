//
// Created by 310165137 on 14/01/2021.
//

#include "topic.h"

unsigned int topic::getNumberofPost() {
    return this->posts.size();
}

unsigned int topic::insertPost(unsigned int postId, const std::string &message) {
    post *npost = new post(postId, message);
    mutex.lock();
    this->posts.push_back(npost);
    mutex.unlock();
    return 0;
}

std::string topic::getPost(unsigned int postId) {
    if (this->getNumberofPost() < postId)
        return "";
    else
        return this->posts[postId]->getMessage();
}

const std::vector<post *> &topic::getPosts() const {
    return posts;
}

bool topic::operator==(const topic &rhs) const {
    return _topic == rhs._topic &&
           posts == rhs.posts;
}

bool topic::operator!=(const topic &rhs) const {
    return !(rhs == *this);
}
