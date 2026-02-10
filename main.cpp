#include "microscope.hpp"

int BinSearch(MicroInt* arr, MicroInt val, MicroInt len);

int main() {
    SetLogLevel(kDebug);

    MicroInt arr[10] = {};

    MICRO_INT(size, 10);
    for (MICRO_INT(i,0); i < size; i++) {
        arr[i] = i;
    }

    MICRO_INT(var, 4);
    fprintf(stdout, "Found index is %d\n", BinSearch(arr, var, size));
}

int BinSearch(MicroInt* arr, MicroInt val, MicroInt size) {
    MICRO_INT(ind, size / 2);

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
