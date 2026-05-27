/**
 * @file search_algorithms.h
 * @brief Заголовочный файл с объявлениями структур, классов и функций поиска.
 *
 * В файле описаны структуры данных и классы, используемые для реализации
 * линейного поиска, бинарного дерева поиска, красно-черного дерева,
 * хэш-таблицы и тестирования времени поиска.
 */

 /**
  * @mainpage Документация проекта
  *
  * @section intro_sec Введение
  *
  * Данный проект реализует сравнение алгоритмов поиска объектов по строковому
  * ключу. Ключом поиска является первое нечисловое поле объекта, то есть поле
  * field1.
  *
  * @section algorithms_sec Реализованные методы поиска
  *
  * В программе реализованы следующие методы:
  * - линейный поиск;
  * - поиск с помощью бинарного дерева поиска;
  * - поиск с помощью красно-черного дерева;
  * - поиск с помощью хэш-таблицы;
  * - поиск с помощью массива multimap.
  *
  * @section result_sec Результаты
  *
  * Результаты тестирования сохраняются в файл res.txt. Файл содержит таблицу
  * с размером массива, временем поиска каждым методом и количеством коллизий
  * хэш-функции.
  */

#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

/**
 * @brief Структура объекта данных.
 *
 * Объект содержит два строковых поля и два числовых значения. В качестве
 * ключа поиска используется первое строковое поле
 */
struct DataObject {
    /** @brief Первое строковое поле, используемое как ключ поиска. */
    string field1;

    /** @brief Второе строковое поле объекта. */
    string field2;

    /** @brief Первое числовое значение объекта. */
    int value1;

    /** @brief Второе числовое значение объекта. */
    int value2;

    /**
     * @brief Конструктор по умолчанию.
     */
    DataObject();

    /**
     * @brief Конструктор с параметрами.
     *
     * @param f1 Первое строковое поле.
     * @param f2 Второе строковое поле.
     * @param v1 Первое числовое значение.
     * @param v2 Второе числовое значение.
     */
    DataObject(const string& f1, const string& f2, int v1, int v2);

    /**
     * @brief Возвращает ключ поиска объекта.
     *
     * @return Значение поля field1.
     */
    string getKey() const;
};

/**
 * @brief Динамический массив указателей на объекты DataObject.
 *
 * Класс используется для хранения исходных данных, промежуточных массивов,
 * результатов поиска и списков объектов с одинаковыми ключами внутри узлов
 * деревьев. Поддерживается ручное управление памятью.
 */
class DynamicArray {
private:
    /** @brief Массив указателей на объекты. */
    DataObject** arr;

    /** @brief Вместимость массива. */
    int capacity;

    /** @brief Текущее количество элементов. */
    int size;

    /** @brief Признак владения объектами внутри массива. */
    bool ownsObjects;

public:
    /**
     * @brief Конструктор динамического массива.
     *
     * @param initialCapacity Начальная вместимость массива.
     * @param ownsObjects Признак владения объектами.
     */
    DynamicArray(int initialCapacity = 10, bool ownsObjects = false);

    /**
     * @brief Деструктор динамического массива.
     *
     * Если массив владеет объектами, то деструктор освобождает память,
     * выделенную под объекты DataObject.
     */
    ~DynamicArray();

    /**
     * @brief Добавляет объект в конец массива.
     *
     * @param obj Указатель на добавляемый объект.
     */
    void push_back(DataObject* obj);

    /**
     * @brief Резервирует новую вместимость массива.
     *
     * @param newCapacity Новая вместимость массива.
     */
    void reserve(int newCapacity);

    /**
     * @brief Возвращает элемент массива по индексу.
     *
     * @param index Индекс элемента.
     * @return Указатель на объект DataObject.
     */
    DataObject* operator[](int index);

    /**
     * @brief Возвращает количество элементов массива.
     *
     * @return Количество элементов.
     */
    int getSize() const;
};

/**
 * @brief Узел дерева поиска.
 *
 * Узел хранит строковый ключ и динамический массив объектов с этим ключом.
 * Такой подход позволяет корректно обрабатывать неуникальные ключи.
 */
struct TreeNode {
    /** @brief Ключ узла. */
    string key;

    /** @brief Массив объектов, имеющих одинаковый ключ. */
    DynamicArray* objects;

    /** @brief Левый потомок. */
    TreeNode* left;

    /** @brief Правый потомок. */
    TreeNode* right;

    /** @brief Родительский узел. */
    TreeNode* parent;

    /** @brief Цвет узла для красно-черного дерева. */
    bool isRed;

    /**
     * @brief Конструктор узла дерева.
     *
     * @param obj Объект, по которому создается узел.
     */
    TreeNode(DataObject* obj);

    /**
     * @brief Деструктор узла дерева.
     */
    ~TreeNode();
};

/**
 * @brief Класс линейного поиска.
 *
 * Выполняет последовательный просмотр всех элементов массива и возвращает
 * все объекты, ключ которых совпадает с заданным.
 */
class LinearSearch {
public:
    /**
     * @brief Выполняет линейный поиск по массиву.
     *
     * @param arr Массив объектов.
     * @param key Ключ поиска.
     * @return Массив найденных объектов.
     */
    static DynamicArray* search(DynamicArray* arr, const string& key);
};

/**
 * @brief Бинарное дерево поиска.
 *
 * Класс реализует хранение объектов по строковому ключу. Повторяющиеся ключи
 * не создают новые узлы, а сохраняются в массиве объектов внутри существующего
 * узла.
 */
class BinarySearchTree {
private:
    /** @brief Корень дерева. */
    TreeNode* root;

public:
    /**
     * @brief Конструктор бинарного дерева поиска.
     */
    BinarySearchTree();

    /**
     * @brief Деструктор бинарного дерева поиска.
     */
    ~BinarySearchTree();

    /**
     * @brief Добавляет объект в дерево.
     *
     * @param obj Указатель на добавляемый объект.
     */
    void insert(DataObject* obj);

    /**
     * @brief Строит дерево по массиву объектов.
     *
     * @param arr Массив объектов.
     */
    void buildFromArray(DynamicArray* arr);

    /**
     * @brief Выполняет поиск объектов по ключу.
     *
     * @param key Ключ поиска.
     * @return Массив найденных объектов.
     */
    DynamicArray* search(const string& key);
};

/**
 * @brief Красно-черное дерево.
 *
 * Класс реализует самобалансирующееся бинарное дерево поиска. Повторяющиеся
 * ключи сохраняются в одном узле дерева.
 */
class RedBlackTree {
private:
    /** @brief Корень дерева. */
    TreeNode* root;

    /**
     * @brief Выполняет левый поворот.
     *
     * @param x Узел, относительно которого выполняется поворот.
     */
    void rotateLeft(TreeNode* x);

    /**
     * @brief Выполняет правый поворот.
     *
     * @param x Узел, относительно которого выполняется поворот.
     */
    void rotateRight(TreeNode* x);

    /**
     * @brief Восстанавливает свойства красно-черного дерева после вставки.
     *
     * @param z Добавленный узел.
     */
    void fixInsert(TreeNode* z);

public:
    /**
     * @brief Конструктор красно-черного дерева.
     */
    RedBlackTree();

    /**
     * @brief Деструктор красно-черного дерева.
     */
    ~RedBlackTree();

    /**
     * @brief Добавляет объект в красно-черное дерево.
     *
     * @param obj Указатель на добавляемый объект.
     */
    void insert(DataObject* obj);

    /**
     * @brief Строит красно-черное дерево по массиву объектов.
     *
     * @param arr Массив объектов.
     */
    void buildFromArray(DynamicArray* arr);

    /**
     * @brief Выполняет поиск объектов по ключу.
     *
     * @param key Ключ поиска.
     * @return Массив найденных объектов.
     */
    DynamicArray* search(const string& key);
};

/**
 * @brief Хэш-таблица с методом цепочек.
 *
 * Класс реализует собственную хэш-функцию и хранение объектов в ячейках
 * таблицы. Коллизии разрешаются методом цепочек с использованием динамических
 * массивов внутри ячеек.
 */
class HashTable {
private:
    /**
     * @brief Ячейка хэш-таблицы.
     *
     * Ячейка содержит динамический массив указателей на объекты, которые
     * попали в один и тот же индекс таблицы.
     */
    struct Bucket {
        /** @brief Массив указателей на объекты. */
        DataObject** data;

        /** @brief Количество объектов в ячейке. */
        int size;

        /** @brief Вместимость ячейки. */
        int capacity;
    };

    /** @brief Массив ячеек хэш-таблицы. */
    Bucket* table;

    /** @brief Размер хэш-таблицы. */
    int tableSize;

    /** @brief Количество коллизий. */
    int collisions;

    /**
     * @brief Вычисляет индекс по строковому ключу.
     *
     * @param key Ключ объекта.
     * @return Индекс ячейки хэш-таблицы.
     */
    int hashFunction(const string& key) const;

    /**
     * @brief Инициализирует ячейку хэш-таблицы.
     *
     * @param bucket Ячейка таблицы.
     */
    void initBucket(Bucket& bucket);

    /**
     * @brief Добавляет объект в ячейку хэш-таблицы.
     *
     * @param bucket Ячейка таблицы.
     * @param obj Указатель на добавляемый объект.
     */
    void addToBucket(Bucket& bucket, DataObject* obj);

public:
    /**
     * @brief Конструктор хэш-таблицы.
     *
     * @param size Размер хэш-таблицы.
     */
    HashTable(int size);

    /**
     * @brief Деструктор хэш-таблицы.
     */
    ~HashTable();

    /**
     * @brief Добавляет объект в хэш-таблицу.
     *
     * @param obj Указатель на добавляемый объект.
     */
    void insert(DataObject* obj);

    /**
     * @brief Строит хэш-таблицу по массиву объектов.
     *
     * @param arr Массив объектов.
     */
    void buildFromArray(DynamicArray* arr);

    /**
     * @brief Выполняет поиск объектов по ключу.
     *
     * @param key Ключ поиска.
     * @return Массив найденных объектов.
     */
    DynamicArray* search(const string& key);

    /**
     * @brief Возвращает количество коллизий.
     *
     * @return Количество коллизий хэш-функции.
     */
    int getCollisions() const;
};

/**
 * @brief Класс тестирования алгоритмов поиска.
 *
 * Класс выполняет построение структур данных, многократный запуск поиска,
 * вычисление среднего времени поиска и сохранение результатов в файл res.txt.
 */
class Benchmark {
private:
    /**
     * @brief Возвращает количество повторов поиска для усреднения времени.
     *
     * @param size Размер тестового массива.
     * @return Количество повторов.
     */
    int getRepeatCount(int size);

public:
    /**
     * @brief Структура результата тестирования.
     *
     * Хранит размер массива, время поиска каждым методом и количество коллизий.
     */
    struct Result {
        /** @brief Размер тестового массива. */
        int size;

        /** @brief Среднее время линейного поиска. */
        double linearTime;

        /** @brief Среднее время поиска в бинарном дереве. */
        double bstTime;

        /** @brief Среднее время поиска в красно-черном дереве. */
        double rbTreeTime;

        /** @brief Среднее время поиска в хэш-таблице. */
        double hashTime;

        /** @brief Среднее время поиска в multimap. */
        double multimapTime;

        /** @brief Количество коллизий хэш-функции. */
        int collisions;
    };

    /**
     * @brief Выполняет тестирование на одном массиве.
     *
     * @param data Тестовый массив.
     * @param searchKey Ключ поиска.
     * @return Указатель на структуру с результатами тестирования.
     */
    Result* runTest(DynamicArray* data, const string& searchKey);

    /**
     * @brief Выполняет тестирование на массивах разных размеров.
     *
     * @param sizes Массив размерностей.
     * @param numSizes Количество размерностей.
     * @param searchKey Ключ поиска.
     * @param originalData Исходный массив данных.
     */
    void runAllTests(int sizes[], int numSizes, const string& searchKey, DynamicArray* originalData);

    /**
     * @brief Сохраняет результаты тестирования в текстовый файл.
     *
     * @param results Массив результатов.
     * @param count Количество результатов.
     * @param filename Имя выходного файла.
     */
    void exportToTXT(Result** results, int count, const string& filename);
};

/**
 * @brief Загружает данные из файла.
 *
 * Файл должен содержать строки формата field1;field2;value1;value2.
 *
 * @param filename Имя файла.
 * @return Массив загруженных объектов.
 */
DynamicArray* loadDataFromFile(const string& filename);

/**
 * @brief Получает ключ поиска из первого объекта массива.
 *
 * @param data Массив данных.
 * @return Ключ поиска.
 */
string getKeyFromData(DynamicArray* data);

#endif