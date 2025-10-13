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
    size_t size_;
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

    size_t size() {
        std::lock_guard<std::mutex> lock(mtx_);
        return q_.size();    
    }
};


class JsonTransporter {
private:
    ThreadSafeQueue<json>& tsq_;
    int server_port_;
    std::string server_host_;
    std::unique_ptr<ix::WebSocketServer> srv_;

    void set_message_callback(std::shared_ptr<ix::WebSocket> srv);
    void set_connection_callback();
public:
    JsonTransporter(ThreadSafeQueue<json>& tsq, std::string server_host,
                    int server_port): tsq_(tsq), 
                    server_host_(server_host),
                    server_port_(server_port) {}
    void start_server();
    void stop_server();
};

