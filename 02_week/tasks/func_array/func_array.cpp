#include <cstddef>
#include <utility>


double ApplyOperations(double a, double b, double (*operations[])(double, double), size_t operations_count) {
    if (operations_count == 0) {
        return 0.0;
    }
    
    double sum {};

    for (size_t i = 0; i < operations_count; ++i) {
        if (operations[i] == nullptr) continue;
        sum += operations[i](std::as_const(a), std::as_const(b));
    }

    return sum;
}