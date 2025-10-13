# Документация по использованию ThreadSafeQueue<T> и JsonTransporter
## ThreadSafeQueue<T>
Потокобезопасная очередь для хранения объектов произвольного типа `T`.

### Методы

- **`void push(const T& obj)`**  
  Добавляет копию объекта в очередь. Потокобезопасно.

- **`void push(T&& obj)`**  
  Добавляет объект в очередь через перемещение. Потокобезопасно.

- **`T pop_front()`**  
  Извлекает первый элемент из очереди и возвращает его.  
  Если очередь пуста — бросает `std::runtime_error`. Потокобезопасно.

- **`size_t size()`**  
  Возвращает текущее количество элементов в очереди. Потокобезопасно.

---

## JsonTransporter

WebSocket-сервер для приёма JSON-сообщений.

### Конструктор

```cpp
JsonTransporter(ThreadSafeQueue<json>& tsq,
                std::string server_host,
                int server_port);
````

* `tsq` — ссылка на потокобезопасную очередь для хранения JSON-сообщений.
* `server_host` — адрес, на котором сервер будет слушать соединения (например, `"0.0.0.0"` для всех интерфейсов или `"127.0.0.1"` только для локальных).
* `server_port` — порт для входящих соединений.

---

### Методы

#### `void start_server()`

Запускает сервер:

1. Инициализирует сетевую подсистему IXWebSocket (`ix::initNetSystem()`).
2. Создаёт объект `ix::WebSocketServer` с заданным хостом и портом.
3. Настраивает callback на подключение клиентов (`set_connection_callback()`).
4. Запускает сервер (`srv_->listen()` + `srv_->start()`).
5. Блокирует текущий поток до остановки (`srv_->wait()`).

#### `void stop_server()`

Останавливает сервер (`srv_->stop()`), закрывая все соединения.

---

### Callback-методы (private)

#### `void set_connection_callback()`

* Вызывается при новых соединениях.
* Получает `std::weak_ptr<ix::WebSocket>` и `std::shared_ptr<ix::ConnectionState>`.
* Если соединение активно, назначает на него обработчик сообщений через `set_message_callback()`.

#### `void set_message_callback(std::shared_ptr<ix::WebSocket> srv)`

* **`Open`** — клиент подключился. Пишет сообщение в консоль.
* **`Message`** — получено новое сообщение.

  * Пытается распарсить строку как `nlohmann::json`.
  * Если успешно — помещает в `ThreadSafeQueue<json>`.
  * Если ошибка парсинга — пишет в консоль причину.
* **`Close`** — клиент отключился.
* **`Error`** — ошибка соединения, выводится причина.

---

## Пример использования

```cpp
#include "json_transporter.h"

int main() {
    ThreadSafeQueue<json> queue;
    JsonTransporter server(queue, "0.0.0.0", 9000);

    // Запуск сервера в отдельном потоке
    std::thread serverThread([&server]() { server.start_server(); });

    // Обработка сообщений
    while (true) {
        try {
            json msg = queue.pop_front();
            std::cout << "Processing JSON: " << msg.dump() << std::endl;
        } catch (std::runtime_error&) {
            // Очередь пуста, подождём немного
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    server.stop_server();
    serverThread.join();
}
```

---

## Особенности

* Использует IXWebSocket для WebSocket-соединений.
* JSON-сообщения обрабатываются через [nlohmann::json](https://github.com/nlohmann/json).
* Все операции с очередью потокобезопасны.
* Поддерживается настройка адреса и порта (`server_host_`, `server_port_`).