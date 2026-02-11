#include "microscope.hpp"

int BinSearch(Micro<int>* arr, int val, int len);

int main() {
    SetLogLevel(kDebug);

    Micro<int> arr[10] = {};

    MICRO(int, size, 10);
    for (MICRO(int, i,0); i < size; i++) {
        arr[i] = i;
    }

    MICRO(int, var, 4);
    fprintf(stdout, "Found index is %d\n", BinSearch(arr, var, size));

    Micro<int>::graph_builder.Draw();
}

int BinSearch(Micro<int>* arr, int val_, int size_) {
    MICRO(int, val, val_);
    MICRO(int, size, size_);
    MICRO(int, ind, size / 2);

    if ((size == 1) && (val != arr[ind])) {
        return -1;
    }

    if (arr[ind] > val) {
        return BinSearch(arr, val, ind);
    }

    if (arr[ind] < val) {
        return BinSearch(arr + ind, val, size - ind) + ind;
    }

    return ind;
}
