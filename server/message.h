#pragma once
#include <fstream>
#include <iostream>
struct Message final {
    std::string sender_;
    std::string receiver_;
    std::string content_;

    Message(const std::string& sender, const std::string& content,
        const std::string& receiver)
        : sender_(sender)
        , content_(content)
        , receiver_(receiver)
    {
    }

    Message() = default;

    void PrintM();

    void operator=(Message& other);
    bool operator==(Message& other);

    friend std::fstream& operator>>(std::fstream& is, Message& obj);
    friend std::ostream& operator<<(std::ostream& os, const Message& obj);
};
