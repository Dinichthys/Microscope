#define RVALUE
#define LVALUE
#define RVALUE_MOVE_FUNC

#include "libs/microscope.hpp"
#include "libs/scope.hpp"
#include "libs/my_std.hpp"

int BinSearch(Micro<int>* arr, Micro<int> val, Micro<int> len);

Micro<int> foo(const Micro<int>& val) {
    return val;
}


#define VAR(var, val) MICRO(int, var, val)

int main() {
    TRACE;

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
    // VAR(a, 0);
    // Micro<int> b(foo(a));
    //---------------------------------------

    // VAR(a, 1);
    // VAR(b, 1);
    // VAR(c, a + b);

    VAR(size, 5);
    Micro<int> arr[5];

    for (VAR(i,0); i < size; i++) {
        MICRO_UPDATENAME(arr[i]);
        arr[i] = i;
    }

    VAR(var, 4);
    fprintf(stdout, "Found index is %d\n", BinSearch(arr, var, size));

    Micro<int>::graph_builder.Draw();
}

int BinSearch(Micro<int>* arr, Micro<int> val, Micro<int> size) {
    TRACE;
    MICRO_UPDATENAME(val);
    MICRO_UPDATENAME(size);
    VAR(ind, size / 2);

    if ((size == 1) && (val != arr[ind])) {
        return -1;
    }

    if (arr[ind] > val) {
#ifdef RVALUE_MOVE_FUNC
        return BinSearch(arr, MyStd::Move(val), MyStd::Move(ind));
#else
        return BinSearch(arr, val, ind);
#endif
    }

    if (arr[ind] < val) {
#ifdef RVALUE_MOVE_FUNC
        return BinSearch(arr + ind, MyStd::Move(val), size - ind) + ind;
#else
        return BinSearch(arr + ind, val, size - ind) + ind;
#endif
    }

    return ind;
}
