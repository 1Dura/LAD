#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class ITransporter {
public:
    virtual std::string receive() = 0;
    virtual void send(std::string data) = 0; 
};


class HttpTransporter: public ITransporter {
public:
    std::string receive() override;
    void send(std::string data) override;
};


class WebSocketTransporter: public ITransporter {
public:
    std::string receive() override;
    void send(std::string data) override;
};