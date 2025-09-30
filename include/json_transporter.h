#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXWebSocketServer.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>
#include <queue>
#include <mutex>

using json = nlohmann::json;

template<class T>
class ThreadSafeQueue {
private:
    std::queue<T> q_;
    mutable std::mutex mtx_;
public:
    ThreadSafeQueue() = default;

    void push(const T& obj) {
        std::lock_guard<std::mutex> lock(mtx_);
        q_.push(obj);
    };

    void push(T&& obj) {
        std::lock_guard<std::mutex> lock(mtx_);
        q_.push(std::move(obj));
    };

    T pop_front() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (q_.empty()) {
            throw std::runtime_error("Queue is empty");
        }
        T el = std::move(q.front());
        q_.pop();
        return el;
    }
};


class JsonTransporter {
private:
    ThreadSafeQueue<json>& tsq_;
    int server_port_;
    std::string server_host_ = "0.0.0.0";
    std::unique_ptr<ix::WebSocketServer> srv_;

    void set_message_callback(std::shared_ptr<ix::WebSocket> srv);
    void set_connection_callback();
public:
    JsonTransporter(ThreadSafeQueue<json>& tsq,
                    int server_port): tsq_(tsq), server_port_(server_port) {}
    void start_server();
    void stop_server();
};

