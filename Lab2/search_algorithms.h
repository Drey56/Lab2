#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

//структура объекта данных
struct DataObject {
    string field1;
    string field2;
    int value1;
    int value2;

    DataObject();
    DataObject(const string& f1, const string& f2, int v1, int v2);
    string getKey() const;
};

//динамический массив объектов
class DynamicArray {
private:
    DataObject** arr;
    int capacity;
    int size;
    bool ownsObjects;

public:
    DynamicArray(int initialCapacity = 10, bool ownsObjects = false);
    ~DynamicArray();

    void push_back(DataObject* obj);
    void reserve(int newCapacity);
    DataObject* operator[](int index);
    int getSize() const;
};

//узел дерева
struct TreeNode {
    string key;
    DynamicArray* objects;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    bool isRed;

    TreeNode(DataObject* obj);
    ~TreeNode();
};

//линейный поиск
class LinearSearch {
public:
    static DynamicArray* search(DynamicArray* arr, const string& key);
};

//бинарное дерево поиска
class BinarySearchTree {
private:
    TreeNode* root;

public:
    BinarySearchTree();
    ~BinarySearchTree();

    void insert(DataObject* obj);
    void buildFromArray(DynamicArray* arr);
    DynamicArray* search(const string& key);
};

//красно-черное дерево
class RedBlackTree {
private:
    TreeNode* root;

    void rotateLeft(TreeNode* x);
    void rotateRight(TreeNode* x);
    void fixInsert(TreeNode* z);

public:
    RedBlackTree();
    ~RedBlackTree();

    void insert(DataObject* obj);
    void buildFromArray(DynamicArray* arr);
    DynamicArray* search(const string& key);
};

//хэш-таблица
class HashTable {
private:
    struct Bucket {
        DataObject** data;
        int size;
        int capacity;
    };

    Bucket* table;
    int tableSize;
    int collisions;

    int hashFunction(const string& key) const;
    void initBucket(Bucket& bucket);
    void addToBucket(Bucket& bucket, DataObject* obj);

public:
    HashTable(int size);
    ~HashTable();

    void insert(DataObject* obj);
    void buildFromArray(DynamicArray* arr);
    DynamicArray* search(const string& key);
    int getCollisions() const;
};

//тестирование алгоритмов
class Benchmark {
private:
    int getRepeatCount(int size);

public:
    struct Result {
        int size;
        double linearTime;
        double bstTime;
        double rbTreeTime;
        double hashTime;
        double multimapTime;
        int collisions;
    };

    Result* runTest(DynamicArray* data, const string& searchKey);
    void runAllTests(int sizes[], int numSizes, const string& searchKey, DynamicArray* originalData);
    void exportToTXT(Result** results, int count, const string& filename);
};

DynamicArray* loadDataFromFile(const string& filename);
string getKeyFromData(DynamicArray* data);

#endif