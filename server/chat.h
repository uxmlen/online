#pragma once

#include <arpa/inet.h>
#include <filesystem>
#include <string>
#include <sys/socket.h>
#include <vector>

#include "user.h"
const int msg_length = 1024;

class Chat final {
public:
    void showMenu();
    void selectAction(int action);
    void showAuthMenu();

    bool isUserExisted(const std::string& login) const;
    bool isUserExisted(const std::string& login, const std::string& pass);

    void signUp();
    void signIn();

    int getIdUserByName(const std::string& name);

private:
    std::vector<User> users_;
    int current_userid_ = 0;
    bool is_authorize_ = false;

    struct sockaddr_in serveraddress, client;
    socklen_t socket_len_;
    int conn_, conn_status_;
    int socket_file_descriptor, bind_status_;

    char msg_receiver_[1024];
    char msg_sender_[1024];
    char msg_content_[1024];
};
