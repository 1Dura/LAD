## Инструкция по сборке проекта LAD_APP

### 1. Создание и переход в каталог сборки

```bash
mkdir build
cd build
```

### 2. Генерация файлов сборки

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

> Здесь `-DCMAKE_BUILD_TYPE=Release` задаёт тип сборки. Для отладочной сборки используйте `Debug`.

### 3. Сборка проекта

```bash
cmake --build . --target LAD_APP
```

* После сборки исполняемый файл `LAD_APP` появится в `./build/Release` (Windows) или `./build` (Linux/macOS).

---

## 4. Сборка и запуск тестов (опционально)

1. Переключите опцию сборки тестов при необходимости:

```bash
cmake .. -DBUILD_TESTS=ON
```

2. Сборка тестов:

```bash
cmake --build . --target TEST_APP
```

3. Запуск тестов:

* С отчётом в формате JSON:

```bash
./TEST_APP --gtest_output=json:report.json
```

* Без отчёта:

```bash
./TEST_APP
```

* Через `ctest` (подходит для CI):

```bash
ctest --output-on-failure
```

---

## 7. Добавление новых файлов

* Любые новые `.cpp` или `.h` файлы необходимо:

  1. Добавить в `add_library(lad_lib ...)` или `add_executable(...)`.
  2. Убедиться, что заголовочные файлы добавлены в `target_include_directories`.
