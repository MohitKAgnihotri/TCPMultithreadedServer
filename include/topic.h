//
// Created by 310165137 on 14/01/2021.
//

#ifndef TCP_CLIENT_SERVER_TOPIC_H
#define TCP_CLIENT_SERVER_TOPIC_H

#include <utility>
#include <mutex>
#include "post.h"

class topic {
private:
    std::string _topic;
    std::vector<post *> posts;
    static std::mutex mutex_post;
public:
    static const std::mutex &getMutexPost() {
        return mutex_post;
    }

public:
    const std::vector<post *> &getPosts() const;

public:
    topic(std::string topic) : _topic(std::move(topic)) {}

    const std::string &getTopic() const {
        return _topic;
    }

    unsigned int insertPost(unsigned int, const std::string &);

    unsigned int getNumberofPost();

    std::string getPost(unsigned int postId);

    bool operator==(const topic &rhs) const;

    bool operator!=(const topic &rhs) const;
};
#endif //TCP_CLIENT_SERVER_TOPIC_H
