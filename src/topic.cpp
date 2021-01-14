//
// Created by 310165137 on 14/01/2021.
//

#include "topic.h"

unsigned int topic::getNumberofPost() {
    return this->posts.size();
}

unsigned int topic::insertPost(unsigned int postId, const std::string &message) {
    post npost(postId, message);
    //this->mutex_post.lock();
    this->posts.push_back(npost);
    //this->mutex_post.unlock();
    return 0;
}

std::string topic::getPost(unsigned int postId) {
    if (this->getNumberofPost() < postId)
        return "";
    else
        return this->posts[postId].getMessage();
}
