#include <cmath>
#include <iomanip>
#include <numbers>
#include <ostream>
#include <algorithm>
#include <array>


struct ExpTag {};
struct DegTag {};
struct AlgTag {};

static constexpr const double EPSILON { 1e-12 }; // std::numeric_limits<double>::epsilon()

static bool Eq(double a, double b) {
    static constexpr const double eps { EPSILON };
    const std::array<double, 3> vals { 1.0, std::abs(a), std::abs(b) };
    const double eps_scaled { eps * *std::max_element(vals.begin(), vals.end()) };
    return std::abs(a - b) <= eps_scaled;
}

class Phasor {
    inline static constexpr const double pi { std::numbers::pi };

public:
    Phasor() = default;

    Phasor(const double magnitude, const double phase_rad) 
        : real_(magnitude * std::cos(phase_rad))
        , imag_(magnitude * std::sin(phase_rad))
    {}

    Phasor(const double magnitude, const double phase_rad, const ExpTag&) 
        : Phasor(magnitude, phase_rad)
    {}

    Phasor(const double magnitude, const double phase_deg, const DegTag&) 
        : Phasor(magnitude, DegreeToRadian(phase_deg))
    {}

    Phasor(const double real, const double imag, const AlgTag&) 
        : real_(real)
        , imag_(imag) 
    {}

    ~Phasor() = default;

    Phasor(const Phasor& other) = default;
    Phasor& operator=(const Phasor& other) = default;
    
    Phasor(Phasor&& other) noexcept = default;
    Phasor& operator=(Phasor&& other) noexcept = default;

    static double RadianToDegree(const double rad) noexcept {
        return rad * 180.0 / pi;
    }
    static double DegreeToRadian(const double deg) noexcept {
        return deg * pi / 180.0;
    }

    /// @name Getters
    /// @{
    double Real() const noexcept {
        return real_;
    }
    double Imag() const noexcept {
        return imag_;
    }

    double Magnitude() const noexcept {
        return std::sqrt(Real() * Real() + Imag() * Imag());
    }
    double Phase() const noexcept {
        const auto phase { std::atan2(Imag(), Real()) };
        return Eq(phase, -pi) ? pi : phase;
    }
    double PhaseDeg() const noexcept {
        return RadianToDegree(Phase());
    }

    double Abs() const noexcept {
        return Magnitude();
    }
    double Angle() const noexcept {
        return Phase();
    }
    double AngleDeg() const noexcept {
        return PhaseDeg(); 
    }

    Phasor Conj() const noexcept {
        return Phasor(Real(), -Imag(), AlgTag{});
    }
    Phasor Inv() const noexcept {
        const double denom { Real() * Real() + Imag() * Imag() };
        return Phasor(Real() / denom, -Imag() / denom, AlgTag{});
    }
    /// @}

    /// @name Setters
    /// @{
    void SetPolar(const double magnitude, const double phase_rad) noexcept {
        real_ = magnitude * std::cos(phase_rad);
        imag_ = magnitude * std::sin(phase_rad);
    }   
    void SetCartesian(const double real, const double imag) noexcept {
        real_ = real;
        imag_ = imag;
    }
    /// @}

    /// @name Operators
    /// @{
    bool operator==(const Phasor& other) const noexcept {
        return Eq(real_, other.real_) && Eq(imag_, other.imag_);
    }
    bool operator!=(const Phasor& other) const noexcept {
        return !(*this == other);
    }

    bool operator==(const double other) const noexcept {
        return Eq(real_, other) && Eq(imag_, 0.0);
    }
    bool operator!=(const double other) const noexcept {
        return !(*this == other);
    }

    Phasor operator+(const double other) const noexcept {
        return Phasor(Real() + other, Imag(), AlgTag{});
    }
    Phasor operator-(const double other) const noexcept {
        return Phasor(Real() - other, Imag(), AlgTag{});
    }
    Phasor operator*(const double other) const noexcept {
        return Phasor(Magnitude() * other, Phase());
    }
    Phasor operator/(const double other) const noexcept {
        return Phasor(Magnitude() / other, Phase());
    }

    friend Phasor operator+(const double lhs, const Phasor& rhs) noexcept {
        return rhs + lhs;
    }
    friend Phasor operator-(const double lhs, const Phasor& rhs) noexcept {
        return Phasor(lhs - rhs.Real(), -rhs.Imag(), AlgTag{});
    }
    friend Phasor operator*(const double lhs, const Phasor& rhs) noexcept {
        return rhs * lhs;
    }
    friend Phasor operator/(const double lhs, const Phasor& rhs) noexcept {
        return Phasor(lhs / rhs.Magnitude(), -rhs.Phase());
    }

    Phasor operator+(const Phasor& other) const noexcept {
        return Phasor(Real() + other.Real(), Imag() + other.Imag(), AlgTag{});
    }
    Phasor operator-(const Phasor& other) const noexcept {
        return Phasor(Real() - other.Real(), Imag() - other.Imag(), AlgTag{});
    }
    Phasor operator*(const Phasor& other) const noexcept { 
        return Phasor(Magnitude() * other.Magnitude(), Phase() + other.Phase());
    }
    Phasor operator/(const Phasor& other) const noexcept {
        return Phasor(Magnitude() / other.Magnitude(), Phase() - other.Phase());
    }

    Phasor& operator+=(const double other) noexcept {
        SetCartesian(Real() + other, Imag());
        return *this;
    }
    Phasor& operator-=(const double other) noexcept {
        SetCartesian(Real() - other, Imag());
        return *this;
    }
    Phasor& operator*=(const double other) noexcept {
        SetPolar(Magnitude() * other, Phase());
        return *this;
    }
    Phasor& operator/=(const double other) noexcept {
        SetPolar(Magnitude() / other, Phase());
        return *this;
    }

    Phasor& operator+=(const Phasor& other) noexcept {
        SetCartesian(Real() + other.Real(), Imag() + other.Imag());
        return *this;
    }
    Phasor& operator-=(const Phasor& other) noexcept {
        SetCartesian(Real() - other.Real(), Imag() - other.Imag());
        return *this;
    }
    Phasor& operator*=(const Phasor& other) noexcept {
        SetPolar(Magnitude() * other.Magnitude(), Phase() + other.Phase());
        return *this;
    }
    Phasor& operator/=(const Phasor& other) noexcept {
        SetPolar(Magnitude() / other.Magnitude(), Phase() - other.Phase());
        return *this;
    }
    /// @}

    /// @name Unary operators
    /// @{
    Phasor operator-() const noexcept {
        return Phasor(-Real(), -Imag(), AlgTag{});
    }
    Phasor operator+() const noexcept {
        return *this;
    }
    /// @}

private:
    double real_{0.0};
    double imag_{0.0};
};

Phasor MakePhasorPolar(const double magnitude, const double phase_rad) {
    return Phasor(magnitude, phase_rad, ExpTag{});
}

Phasor MakePhasorPolarDeg(const double magnitude, const double phase_deg) {
    return Phasor(magnitude, phase_deg, DegTag{});
}

Phasor MakePhasorCartesian(const double real, const double imag) {
    return Phasor(real, imag, AlgTag{});
}

std::ostream& operator<<(std::ostream& os, const Phasor& phasor) {
    os << std::dec << std::fixed << std::setprecision(3) 
       << phasor.Magnitude() << "*e(j*" << phasor.PhaseDeg() << ")"
       << " [" << phasor.Real() << " + j*" << phasor.Imag() << "]";
    return os;
}