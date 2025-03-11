#include <limits>
#include <stdexcept>

template <typename T>
class SoftNumber {
public:
	T number;

	SoftNumber() {
		number = T(0);
	}

	SoftNumber(T number) : number(number){} // NOLINT

	SoftNumber operator+(const SoftNumber<T>& other) {
		return AddWithOverflowCheck(number, other.number);
	}

	SoftNumber operator-(const SoftNumber<T>& other) {
		return AddWithOverflowCheck(number, -other.number);
	}

	SoftNumber operator-() const {
		return MultiplyWithOverflowCheck(number, -1);
	}

	SoftNumber operator*(const SoftNumber<T>& other) {
		return MultiplyWithOverflowCheck(number, SoftNumber<T>(other).number);
	}

	SoftNumber operator/(const SoftNumber<T>& other) {
		return number / other;
	}

	SoftNumber& operator+=(const SoftNumber& other) {
		*this = *this + other;
		return *this;
	}

	SoftNumber& operator-=(const SoftNumber& other) {
		*this = *this - other;
		return *this;
	}

	SoftNumber& operator*=(const SoftNumber& other) {
		*this = *this * other;
		return *this;
	}

	SoftNumber& operator/=(const SoftNumber& other) {
		*this = *this / other;
		return *this;
	}

	operator T() { // NOLINT
		return number;
	}

	friend std::ostream& operator<<(std::ostream& os, const SoftNumber& obj) {
		return os << obj.number;
	};

private:
	T AddWithOverflowCheck(T a, T b) {
		if (
			b > 0 && a > std::numeric_limits<T>::max() - b
			|| b < 0 && a < std::numeric_limits<T>::min() - b
		) {
			throw std::overflow_error("Overflow during addition");
		}
		return a + b;
	}

	T MultiplyWithOverflowCheck(T a, T b) {
		if (
			a < 0 && b < 0 && a < std::numeric_limits<T>::max() / b
			|| a < 0 && b > 0 && a < std::numeric_limits<T>::min() / b
			|| a > 0 && b < 0 && b < std::numeric_limits<T>::min() / a
			|| a > 0 && b > 0 && a > std::numeric_limits<T>::max() / b
		) {
			throw std::overflow_error("Overflow during multiplication");
		}
		return a * b;
	}
};