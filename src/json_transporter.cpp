#include "json_transporter.h"


void JsonTransporter::set_message_callback(std::shared_ptr<ix::WebSocket> srv) {
    srv->setOnMessageCallback([this](const ix::WebSocketMessagePtr &msg)
        {
            if (msg->type == ix::WebSocketMessageType::Open) {
                std::cout << "The client has connected." << std::endl;
            }
            else if (msg->type == ix::WebSocketMessageType::Message) {
                try {
                    std::cout << "Received message: " << msg->str << std::endl;
                    json j = json::parse(msg->str);
                    tsq_.push(std::move(j));
                }
                catch (const std::exception& ex) {
                    std::cout << "JSON parsing error: " << ex.what() << std::endl;
                }
            }
            else if (msg->type == ix::WebSocketMessageType::Close) {
                std::cout << "The client has disconnected." << std::endl;
            }
            else if (msg->type == ix::WebSocketMessageType::Error) {
                std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
            }
        }
    ); 
}

void JsonTransporter::set_connection_callback() {
    srv_->setOnConnectionCallback([this](std::weak_ptr<ix::WebSocket> webSocket,
        std::shared_ptr<ix::ConnectionState> conState) {
            auto ws = webSocket.lock();
            if (ws) {
                set_message_callback(ws);
            }
        }
    );
}

void JsonTransporter::start_server() {
    ix::initNetSystem();
    srv_ = std::make_unique<ix::WebSocketServer>(server_port_, server_host_);
    std::cout << "JsonTransporter server started on " << server_port_ << " port";
    set_connection_callback();
    
    auto res = srv_->listen();
    if (!res.first) {
        throw std::runtime_error("Server start error");
    }
    srv_->start();
    srv_->wait();
}

void JsonTransporter::stop_server() {
    srv_->stop();
}