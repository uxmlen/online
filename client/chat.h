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

    int socket_file_descriptor, conn_;
    struct sockaddr_in serveraddress, client;
    char msg_content_[1024];
};
