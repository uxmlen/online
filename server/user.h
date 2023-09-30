#pragma once

#include <fstream>
#include <vector>

#include "message.h"

class User {
public:
    User(const std::string& username, const std::string& login,
        const std::string& password)
        : username_(username)
        , login_(login)
        , password_(password)
    {
    }

    User() = default;

    void setUsername(const std::string& name);
    // getter
    std::string getPassword();
    std::string getLogin();
    std::string getUsername();

    void pushMessage(const Message& message); // получение сообщения

    bool isEmpty(); // получение наличия сообщений
    void PrintMessages(); // чтение сообщений

    friend std::fstream& operator>>(std::fstream& is, User& obj);
    friend std::ostream& operator<<(std::ostream& os, const User& obj);

    bool operator==(User& other)
    {
        if (login_ == other.getLogin()) {
            return true;
        }
        return false;
    }

    void operator=(User& other)
    {
        login_ = other.getLogin();
        username_ = other.getUsername();
        password_ = other.getPassword();
    }

    void sendMessage();

private:
    std::string username_;
    std::string login_;
    std::string password_;
    std::vector<Message> messages_;
};
