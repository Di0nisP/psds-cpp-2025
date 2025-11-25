#include <cstddef>
#include <stdexcept>
#include <span>
#include <cmath>


double CalculateRMS(double values[], size_t size) {
    if (size == 0 || values == nullptr) {
        return 0.0;
    }
    
    std::span<double> dataSpan(values, size);
    
    double rms {};
    for (const auto& value : dataSpan) {
        if (!std::isfinite(value)) {
            throw std::invalid_argument("Array contains non-finite values");
        }
        rms += value * value;
    }

    return std::sqrt(rms / static_cast<double>(size));
}