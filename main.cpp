#define RVALUE
#define LVALUE
#define RVALUE_MOVE_FUNC

#include "libs/microscope.hpp"

int BinSearch(Micro<int>* arr, Micro<int> val, Micro<int> len);

Micro<int> foo(const Micro<int>& val) {
    return val;
}

int main() {
    SetLogLevel(kDebug);

    // UB constructor Example:
    //---------------------------------------
    // Micro<int> a(0, "a", "main");
    // std::cout << a.GetName() << std::endl;
    // Micro<int> b(a, true);
    // std::cout << a.GetName() << std::endl;
    //---------------------------------------

    // Example with copy constructor:
    //---------------------------------------
    // MICRO(int, a, 0);
    // Micro<int> b(foo(a));
    //---------------------------------------

    MICRO(int, a, 1);
    MICRO(int, b, 1);
    MICRO(int, c, a + b);

    MICRO(int, size, 5);
    Micro<int> arr[5];

    for (MICRO(int, i,0); i < size; i++) {
        MICRO_UPDATENAME(arr[i]);
        arr[i] = i;
    }

    MICRO(int, var, 4);
    fprintf(stdout, "Found index is %d\n", BinSearch(arr, var, size));

    Micro<int>::graph_builder.Draw();
}

int BinSearch(Micro<int>* arr, Micro<int> val, Micro<int> size) {
    MICRO_UPDATENAME(val);
    MICRO_UPDATENAME(size);
    MICRO(int, ind, size / 2);

    if ((size == 1) && (val != arr[ind])) {
        return -1;
    }

    if (arr[ind] > val) {
#ifdef RVALUE_MOVE_FUNC
        return BinSearch(arr, std::move(val), std::move(ind));
#else
        return BinSearch(arr, val, ind);
#endif
    }

    if (arr[ind] < val) {
#ifdef RVALUE_MOVE_FUNC
        return BinSearch(arr + ind, std::move(val), size - ind) + ind;
#else
        return BinSearch(arr + ind, val, size - ind) + ind;
#endif
    }

    return ind;
}
