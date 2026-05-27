/**
 * @file search_algorithms.cpp
 * @brief Файл реализации структур данных, алгоритмов поиска и тестирования.
 *
 * В файле реализованы динамический массив, бинарное дерево поиска,
 * красно-черное дерево, хэш-таблица, загрузка данных из файла и сохранение
 * результатов тестирования.
 */

#include "search_algorithms.h"
#include <map>
#include <cstdlib>
#include <iomanip>

DataObject::DataObject() : value1(0), value2(0) {
}

DataObject::DataObject(const string& f1, const string& f2, int v1, int v2)
    : field1(f1), field2(f2), value1(v1), value2(v2) {
}

string DataObject::getKey() const {
    return field1;
}

DynamicArray::DynamicArray(int initialCapacity, bool ownsObjects) {
    capacity = initialCapacity;
    size = 0;
    this->ownsObjects = ownsObjects;
    arr = new DataObject * [capacity];
}

DynamicArray::~DynamicArray() {
    if (ownsObjects) {
        for (int i = 0; i < size; i++) {
            delete arr[i];
        }
    }

    delete[] arr;
}

void DynamicArray::reserve(int newCapacity) {
    if (newCapacity <= capacity) {
        return;
    }

    DataObject** newArr = new DataObject * [newCapacity];

    for (int i = 0; i < size; i++) {
        newArr[i] = arr[i];
    }

    delete[] arr;

    arr = newArr;
    capacity = newCapacity;
}

void DynamicArray::push_back(DataObject* obj) {
    if (size >= capacity) {
        int newCapacity = capacity * 2;

        DataObject** newArr = new DataObject * [newCapacity];

        for (int i = 0; i < size; i++) {
            newArr[i] = arr[i];
        }

        delete[] arr;

        arr = newArr;
        capacity = newCapacity;
    }

    arr[size] = obj;
    size++;
}

DataObject* DynamicArray::operator[](int index) {
    return arr[index];
}

int DynamicArray::getSize() const {
    return size;
}

TreeNode::TreeNode(DataObject* obj) {
    key = obj->getKey();
    objects = new DynamicArray(2, false);
    objects->push_back(obj);
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    isRed = true;
}

TreeNode::~TreeNode() {
    delete objects;
}

/**
 * @brief Добавляет узел во вспомогательный стек.
 *
 * Функция используется при нерекурсивном удалении деревьев.
 *
 * @param stack Массив указателей на узлы.
 * @param top Индекс вершины стека.
 * @param capacity Вместимость стека.
 * @param node Добавляемый узел.
 */
static void pushTreeNode(TreeNode**& stack, int& top, int& capacity, TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    if (top >= capacity) {
        int newCapacity = capacity * 2;

        TreeNode** newStack = new TreeNode * [newCapacity];

        for (int i = 0; i < top; i++) {
            newStack[i] = stack[i];
        }

        delete[] stack;

        stack = newStack;
        capacity = newCapacity;
    }

    stack[top] = node;
    top++;
}

/**
 * @brief Удаляет дерево без использования рекурсии.
 *
 * Нерекурсивное удаление используется для предотвращения переполнения стека
 * при большом количестве узлов.
 *
 * @param root Корень удаляемого дерева.
 */
static void destroyTreeNodes(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    int capacity = 1024;
    int top = 0;

    TreeNode** stack = new TreeNode * [capacity];

    pushTreeNode(stack, top, capacity, root);

    while (top > 0) {
        TreeNode* current = stack[top - 1];
        top--;

        pushTreeNode(stack, top, capacity, current->left);
        pushTreeNode(stack, top, capacity, current->right);

        delete current;
    }

    delete[] stack;
}

DynamicArray* LinearSearch::search(DynamicArray* arr, const string& key) {
    DynamicArray* results = new DynamicArray(10, false);

    for (int i = 0; i < arr->getSize(); i++) {
        if ((*arr)[i]->getKey() == key) {
            results->push_back((*arr)[i]);
        }
    }

    return results;
}

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

BinarySearchTree::~BinarySearchTree() {
    destroyTreeNodes(root);
}

void BinarySearchTree::insert(DataObject* obj) {
    if (root == nullptr) {
        root = new TreeNode(obj);
        return;
    }

    TreeNode* current = root;
    TreeNode* parent = nullptr;
    string key = obj->getKey();

    while (current != nullptr) {
        parent = current;

        if (key == current->key) {
            current->objects->push_back(obj);
            return;
        }

        if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    TreeNode* newNode = new TreeNode(obj);
    newNode->parent = parent;

    if (key < parent->key) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }
}

void BinarySearchTree::buildFromArray(DynamicArray* arr) {
    for (int i = 0; i < arr->getSize(); i++) {
        insert((*arr)[i]);
    }
}

DynamicArray* BinarySearchTree::search(const string& key) {
    DynamicArray* results = new DynamicArray(10, false);

    TreeNode* current = root;

    while (current != nullptr) {
        if (key == current->key) {
            for (int i = 0; i < current->objects->getSize(); i++) {
                results->push_back((*(current->objects))[i]);
            }

            return results;
        }

        if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return results;
}

RedBlackTree::RedBlackTree() {
    root = nullptr;
}

RedBlackTree::~RedBlackTree() {
    destroyTreeNodes(root);
}

void RedBlackTree::rotateLeft(TreeNode* x) {
    TreeNode* y = x->right;

    x->right = y->left;

    if (y->left != nullptr) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == nullptr) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void RedBlackTree::rotateRight(TreeNode* x) {
    TreeNode* y = x->left;

    x->left = y->right;

    if (y->right != nullptr) {
        y->right->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == nullptr) {
        root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

void RedBlackTree::fixInsert(TreeNode* z) {
    while (z != root && z->parent->isRed) {
        if (z->parent == z->parent->parent->left) {
            TreeNode* y = z->parent->parent->right;

            if (y != nullptr && y->isRed) {
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(z);
                }

                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                rotateRight(z->parent->parent);
            }
        }
        else {
            TreeNode* y = z->parent->parent->left;

            if (y != nullptr && y->isRed) {
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }

                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                rotateLeft(z->parent->parent);
            }
        }
    }

    root->isRed = false;
}

void RedBlackTree::insert(DataObject* obj) {
    TreeNode* y = nullptr;
    TreeNode* x = root;

    string key = obj->getKey();

    while (x != nullptr) {
        y = x;

        if (key == x->key) {
            x->objects->push_back(obj);
            return;
        }

        if (key < x->key) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }

    TreeNode* z = new TreeNode(obj);
    z->parent = y;

    if (y == nullptr) {
        root = z;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }

    fixInsert(z);
}

void RedBlackTree::buildFromArray(DynamicArray* arr) {
    for (int i = 0; i < arr->getSize(); i++) {
        insert((*arr)[i]);
    }
}

DynamicArray* RedBlackTree::search(const string& key) {
    DynamicArray* results = new DynamicArray(10, false);

    TreeNode* current = root;

    while (current != nullptr) {
        if (key == current->key) {
            for (int i = 0; i < current->objects->getSize(); i++) {
                results->push_back((*(current->objects))[i]);
            }

            return results;
        }

        if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return results;
}

HashTable::HashTable(int size) {
    tableSize = size;
    table = new Bucket[tableSize];
    collisions = 0;

    for (int i = 0; i < tableSize; i++) {
        initBucket(table[i]);
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < tableSize; i++) {
        delete[] table[i].data;
    }

    delete[] table;
}

void HashTable::initBucket(Bucket& bucket) {
    bucket.capacity = 0;
    bucket.size = 0;
    bucket.data = nullptr;
}

void HashTable::addToBucket(Bucket& bucket, DataObject* obj) {
    if (bucket.capacity == 0) {
        bucket.capacity = 4;
        bucket.data = new DataObject * [bucket.capacity];
    }

    if (bucket.size >= bucket.capacity) {
        int newCapacity = bucket.capacity * 2;

        DataObject** newData = new DataObject * [newCapacity];

        for (int i = 0; i < bucket.size; i++) {
            newData[i] = bucket.data[i];
        }

        delete[] bucket.data;

        bucket.data = newData;
        bucket.capacity = newCapacity;
    }

    bucket.data[bucket.size] = obj;
    bucket.size++;
}

int HashTable::hashFunction(const string& key) const {
    unsigned long hash = 5381;

    for (size_t i = 0; i < key.length(); i++) {
        hash = ((hash << 5) + hash) + key[i];
    }

    return hash % tableSize;
}

void HashTable::insert(DataObject* obj) {
    int index = hashFunction(obj->getKey());

    if (table[index].size > 0) {
        collisions++;
    }

    addToBucket(table[index], obj);
}

void HashTable::buildFromArray(DynamicArray* arr) {
    for (int i = 0; i < arr->getSize(); i++) {
        insert((*arr)[i]);
    }
}

DynamicArray* HashTable::search(const string& key) {
    DynamicArray* results = new DynamicArray(10, false);

    int index = hashFunction(key);

    for (int i = 0; i < table[index].size; i++) {
        if (table[index].data[i]->getKey() == key) {
            results->push_back(table[index].data[i]);
        }
    }

    return results;
}

int HashTable::getCollisions() const {
    return collisions;
}

int Benchmark::getRepeatCount(int size) {
    if (size <= 1000) {
        return 1000;
    }

    if (size <= 10000) {
        return 300;
    }

    if (size <= 50000) {
        return 100;
    }

    return 50;
}

Benchmark::Result* Benchmark::runTest(DynamicArray* data, const string& searchKey) {
    Result* result = new Result();

    result->size = data->getSize();

    int repeats = getRepeatCount(data->getSize());

    clock_t start;

    start = clock();

    for (int i = 0; i < repeats; i++) {
        DynamicArray* linearResults = LinearSearch::search(data, searchKey);
        delete linearResults;
    }

    result->linearTime = double(clock() - start) / CLOCKS_PER_SEC * 1000 / repeats;

    BinarySearchTree bst;
    bst.buildFromArray(data);

    start = clock();

    for (int i = 0; i < repeats; i++) {
        DynamicArray* bstResults = bst.search(searchKey);
        delete bstResults;
    }

    result->bstTime = double(clock() - start) / CLOCKS_PER_SEC * 1000 / repeats;

    RedBlackTree rbt;
    rbt.buildFromArray(data);

    start = clock();

    for (int i = 0; i < repeats; i++) {
        DynamicArray* rbtResults = rbt.search(searchKey);
        delete rbtResults;
    }

    result->rbTreeTime = double(clock() - start) / CLOCKS_PER_SEC * 1000 / repeats;

    HashTable ht(data->getSize() * 2);
    ht.buildFromArray(data);

    start = clock();

    for (int i = 0; i < repeats; i++) {
        DynamicArray* hashResults = ht.search(searchKey);
        delete hashResults;
    }

    result->hashTime = double(clock() - start) / CLOCKS_PER_SEC * 1000 / repeats;
    result->collisions = ht.getCollisions();

    multimap<string, DataObject*>* mmap = new multimap<string, DataObject*>();

    for (int i = 0; i < data->getSize(); i++) {
        mmap->insert(pair<string, DataObject*>((*data)[i]->getKey(), (*data)[i]));
    }

    volatile int controlCounter = 0;

    start = clock();

    for (int i = 0; i < repeats; i++) {
        pair<multimap<string, DataObject*>::iterator, multimap<string, DataObject*>::iterator> range = mmap->equal_range(searchKey);

        for (multimap<string, DataObject*>::iterator it = range.first; it != range.second; ++it) {
            controlCounter++;
        }
    }

    result->multimapTime = double(clock() - start) / CLOCKS_PER_SEC * 1000 / repeats;

    delete mmap;

    return result;
}

void Benchmark::runAllTests(int sizes[], int numSizes, const string& searchKey, DynamicArray* originalData) {
    Result** results = new Result * [numSizes];

    for (int i = 0; i < numSizes; i++) {
        cout << "Rows: " << sizes[i] << endl;

        DynamicArray* testData = new DynamicArray(sizes[i], false);

        for (int j = 0; j < sizes[i]; j++) {
            testData->push_back((*originalData)[j]);
        }

        results[i] = runTest(testData, searchKey);

        delete testData;
    }

    exportToTXT(results, numSizes, "res.txt");

    for (int i = 0; i < numSizes; i++) {
        delete results[i];
    }

    delete[] results;
}

void Benchmark::exportToTXT(Result** results, int count, const string& filename) {
    ofstream file(filename.c_str());

    file << fixed << setprecision(6);

    file << "Размер;Линейный_поиск_мс;Бинарное_дерево_мс;Красно_черное_дерево_мс;Хэш_таблица_мс;Ассоциативный_массив_мс;Коллизии" << endl;

    for (int i = 0; i < count; i++) {
        file << results[i]->size << ";"
            << results[i]->linearTime << ";"
            << results[i]->bstTime << ";"
            << results[i]->rbTreeTime << ";"
            << results[i]->hashTime << ";"
            << results[i]->multimapTime << ";"
            << results[i]->collisions << endl;
    }

    file.close();
}

DynamicArray* loadDataFromFile(const string& filename) {
    DynamicArray* data = new DynamicArray(10, true);

    fstream file(filename.c_str(), ios::in);

    string line;

    while (getline(file, line)) {
        if (line.length() > 0 && line[line.length() - 1] == '\r') {
            line.erase(line.length() - 1);
        }

        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        size_t pos3 = line.find(';', pos2 + 1);

        string field1 = line.substr(0, pos1);
        string field2 = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string val1_str = line.substr(pos2 + 1, pos3 - pos2 - 1);
        string val2_str = line.substr(pos3 + 1);

        int val1 = atoi(val1_str.c_str());
        int val2 = atoi(val2_str.c_str());

        DataObject* obj = new DataObject(field1, field2, val1, val2);

        data->push_back(obj);
    }

    file.close();

    return data;
}

string getKeyFromData(DynamicArray* data) {
    return (*data)[0]->getKey();
}