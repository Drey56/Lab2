#include "search_algorithms.h"

int main() {

DynamicArray* data = loadDataFromFile("ds.txt");

    cout << "Loaded: " << data->getSize() << endl;

    string searchKey = getKeyFromData(data);

    int sizes[] = {100, 500, 1000, 5000, 10000, 25000, 50000, 75000, 90000, 100000};

    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    Benchmark benchmark;
    benchmark.runAllTests(sizes, numSizes, searchKey, data);

    delete data;

    cout << "Gotovo)" << endl;

    return 0;
}