/**
 * @file main.cpp
 * @brief Главный файл программы для запуска сравнения алгоритмов поиска.
 *
 * Файл содержит точку входа в программу. В main выполняется загрузка данных
 * из файла ds.txt, выбор ключа поиска, запуск тестирования алгоритмов и
 * сохранение результатов в файл res.txt.
 */

#include "search_algorithms.h"

 /**
  * @brief Точка входа в программу.
  *
  * Выполняет загрузку исходных данных, задает размеры тестовых массивов,
  * запускает тестирование алгоритмов поиска и освобождает выделенную память.
  *
  * @return Код завершения программы.
  */
int main() {

    DynamicArray* data = loadDataFromFile("ds.txt");

    cout << "Loaded: " << data->getSize() << endl;

    string searchKey = getKeyFromData(data);

    int sizes[] = {100, 500, 1000, 5000, 10000, 25000, 50000, 75000, 90000, 100000};

    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    Benchmark benchmark;
    benchmark.runAllTests(sizes, numSizes, searchKey, data);

    delete data;

    cout << "Done" << endl;

    return 0;
}