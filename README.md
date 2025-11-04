# OS tree

## Сборка
### Требования

- C++17
- CMake >= 3.14
- GoogleTest

создание директории сборки
```bash
mkdir build
cd build
```

Все команды ниже исполняются в директории build.

Сборка и запуск тестов:
```bash
cmake .. -DBUILD_TESTS=ON
make
ctest
```

Сборка и компиляция основного приложения:
```bash
cmake .. -DBUILD_TESTS=OFF
make
```

Сборка и компиляция основного приложения с отладочными принтами:
```bash
cmake .. -DBUILD_TESTS=OFF -DDEBUG
make
```


Запуск основного приложения:
```bash
cmake --build . --target run_input
```
что эквивалентно
```bash
./app/tree_app < ../data/input.txt
```

Запуск бенчмарка:
```bash
cmake --build . --target run_benchmark
```
или
```bash
./app/benchmark
```
## Сравнение с std::set

Для оценки эффективности реализации поиска числа узлов с ключами на отрезке [a, b], было проведено сравнение с реализацией через std::set. OS_tree и std::set заполнялись 10000 элементов, после чего производились замеры для подсчета вхождений в 100 000 различных отрезков, идентичных для обоих структур данных. Измерения проводились с помощью std::chrono.

Результаты:

на Node* с рекурсивным удалением дерева:
```bash

--- std::set Results ---
Run 1: Total found 333141511 items in 1994354 microseconds.
Run 2: Total found 333141511 items in 1972853 microseconds.
Run 3: Total found 333141511 items in 1988249 microseconds.
Run 4: Total found 333141511 items in 1987416 microseconds.
Run 5: Total found 333141511 items in 1992508 microseconds.

--- OS_Tree Results ---
Run 1: Total found 333141511 items in 27230 microseconds.
Run 2: Total found 333141511 items in 27048 microseconds.
Run 3: Total found 333141511 items in 27434 microseconds.
Run 4: Total found 333141511 items in 27795 microseconds.
Run 4: Total found 333141511 items in 27795 microseconds.
Run 5: Total found 333141511 items in 27007 microseconds.
```

с unique_ptr:
```bash
Starting 5 runs for std::set with N=10000, M=100000.

--- std::set Results ---
Run 1: Total found 333141511 items in 1939046 microseconds.
Run 2: Total found 333141511 items in 1929063 microseconds.
Run 3: Total found 333141511 items in 1929185 microseconds.
Run 4: Total found 333141511 items in 1923616 microseconds.
Run 5: Total found 333141511 items in 1923076 microseconds.

--- OS_Tree Results ---
Run 1: Total found 333141511 items in 174478 microseconds.
Run 2: Total found 333141511 items in 171684 microseconds.
Run 3: Total found 333141511 items in 174481 microseconds.
Run 4: Total found 333141511 items in 172565 microseconds.
Run 5: Total found 333141511 items in 172229 microseconds.
```

финальная версия с хранением узлов в std::vector:

```bash
Starting 5 runs for std::set with N=10000, M=100000.

--- std::set Results ---
Run 1: Total found 333141511 items in 1866064 microseconds.
Run 2: Total found 333141511 items in 1846617 microseconds.
Run 3: Total found 333141511 items in 1846328 microseconds.
Run 4: Total found 333141511 items in 1853174 microseconds.
Run 5: Total found 333141511 items in 1822880 microseconds.

--- OS_Tree Results ---
Run 1: Total found 333141511 items in 60207 microseconds.
Run 2: Total found 333141511 items in 61777 microseconds.
Run 3: Total found 333141511 items in 61907 microseconds.
Run 4: Total found 333141511 items in 59674 microseconds.
Run 5: Total found 333141511 items in 61394 microseconds.
```
