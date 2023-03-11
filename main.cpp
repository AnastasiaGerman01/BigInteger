#include <iostream>
#include <vector>
#include <string>

class BigInteger;

bool operator<(const BigInteger &a, const BigInteger &b);

bool operator>(const BigInteger &a, const BigInteger &b);

bool operator<=(const BigInteger &a, const BigInteger &b);

bool operator>=(const BigInteger &a, const BigInteger &b);

bool operator==(const BigInteger &a, const BigInteger &b);

bool operator!=(const BigInteger &a, const BigInteger &b);

BigInteger operator*(BigInteger a, const BigInteger &b);

BigInteger operator/(BigInteger a, const BigInteger &b);

std::ostream &operator<<(std::ostream &out, const BigInteger &number);

class BigInteger {
 private:
  std::vector<long long> digits;
  bool sign = true;
  void Increase_1() {
    digits[0] += 1;
    int add = digits[0] / module;
    digits[0] %= module;
    size_t index = 1;
    while (add != 0) {
      if (index < digits.size()) {
        digits[index] = digits[index] + add;
        add = digits[index] / module;
        digits[index] %= module;
      } else {
        digits.push_back(add);
        add = 0;
      }
      index++;
    }
  }
  void Decrease_1() {
    if (digits[digits.size() - 1] == 0) {
      digits[0] = 1;
      sign = false;
    } else {
      int index = 0;
      while (digits[index] == 0) {
        digits[index] = 9;
        index++;
      }
      digits[index] -= 1;
    }
  }
  void right() {
    digits.push_back(0);
    for (int i = digits.size() - 1; i > 0; i--) {
      digits[i] = digits[i - 1];
    }
  }
 public:
  int module = 1e9;
  friend bool operator<(const BigInteger &a, const BigInteger &b);
  friend bool operator==(const BigInteger &a, const BigInteger &b);
  friend std::istream &operator>>(std::istream &in, BigInteger &number);
  BigInteger(const BigInteger &number) {
    sign = number.sign;
    for (int i: number.digits) {
      digits.push_back(i);
    }
  }
  BigInteger(int number) {
    sign = (number >= 0);
    if (number == 0) {
      digits.push_back(0);
    } else {
      number = abs(number);
      while (number > 0) {
        digits.push_back(number % module);
        number /= module;
      }
    }
  }

  BigInteger() {}

  BigInteger (const char *number1): BigInteger(number1, std::strlen(number1)){}

  BigInteger(const char *number1, size_t count) {
    std::string number = number1;
    int index = 0;
    if (number[0] == '-') {
      sign = false;
      index++;
    }
    for (int i = static_cast<int>(count); i > index; i -= 9) {
      if (i > 9 + index) {
        std::string new_digit = number.substr(i - 9, 9);
        digits.push_back(std::stoi(new_digit));
      } else {
        std::string new_digit = number.substr(index, i - index);
        digits.push_back(std::stoi(new_digit));
      }
    }
    if (digits[digits.size() - 1] == 0) {
      sign = true;
    }
  }

  ~BigInteger() {
    digits.clear();
    digits.shrink_to_fit();
  }

  
  BigInteger operator-() const {
    BigInteger copy = *this;
    if (digits[digits.size() - 1] != 0) {
      copy.sign = !(copy.sign);
    }
    return copy;
  }

  explicit operator bool() const {
    if (digits[digits.size() - 1] == 0) {
      return false;
    }
    return true;
  }

  std::string toString() const {
    std::string answer = "";
    if (!sign) {
      answer += '-';
    }
    if (digits[digits.size() - 1] == 0) {
      return "0";
    }
    answer += std::to_string(digits[digits.size() - 1]);
    for (int i = static_cast<int>(digits.size()) - 2; i > -1; i--) {
      std::string digit = std::to_string(digits[i]);
      std::string add = "";
      for (int j = 0; j < 9 - static_cast<int>(digit.size()); j++) {
        add += '0';
      }
      add += digit;
      answer += add;
    }
    return answer;
  }

  BigInteger &operator+=(const BigInteger &number) {
    if (!sign && !number.sign) {
      sign = true;
      *this += -number;
      sign = false;
      if (digits[digits.size() - 1] == 0) {
        sign = true;
      }
      return *this;
    }
    if (!sign) {
      sign = true;
      *this -= number;
      sign ^= 1;
      if (digits[digits.size() - 1] == 0) {
        sign = true;
      }
      return *this;
    }
    if (sign && !number.sign) {
      *this -= -number;
      if (digits[digits.size() - 1] == 0) {
        sign = true;
      }
      return *this;
    }
    int64_t add = 0;
    for (size_t i = 0; i < std::max(digits.size(), number.digits.size()); i++) {
      if (i >= digits.size()) {
        digits.push_back(0);
      }
      int64_t count = 0;
      if (i < number.digits.size()) {
        count = number.digits[i];
      }
      digits[i] += count + add;
      add = digits[i] / module;
      digits[i] %= module;
    }
    if (add != 0) {
      digits.push_back(add);
    }
    if (digits[digits.size() - 1] == 0) {
      sign = true;
    }
    return *this;
  }

  BigInteger &operator-=(const BigInteger &number) {
    if (!sign && !number.sign) {
      sign = true;
      *this -= -number;
      sign ^= 1;
      if (digits[digits.size() - 1] == 0) {
        sign = true;
      }
      return *this;
    }
    if (!sign) {
      sign = true;
      *this += number;
      sign = false;
      if (digits[digits.size() - 1] == 0) {
        sign = true;
      }
      return *this;
    }
    if (sign && !number.sign) {
      *this += -number;
      if (digits[digits.size() - 1] == 0) {
        sign = true;
      }
      return *this;
    }
    if (*this >= number) {
      int check = 0;
      for (size_t i = 0; i < digits.size(); i++) {
        int64_t count = 0;
        if (i < number.digits.size()) {
          count = number.digits[i];
        }
        digits[i] -= check + count;
        if (digits[i] < 0) {
          digits[i] += module;
          check = 1;
        } else {
          check = 0;
        }
      }
    } else {
      BigInteger copy(number);
      copy -= *this;
      *this = copy;
      sign = false;
    }
    while (digits.size() != 1 && digits[digits.size() - 1] == 0) {
      digits.pop_back();
    }
    if (digits[digits.size() - 1] == 0) {
      sign = true;
    }
    return *this;
  }

  BigInteger &operator*=(const BigInteger &number) {
    if (digits[digits.size() - 1] == 0) {
      return *this;
    }
    if (number.digits[number.digits.size() - 1] == 0) {
      sign = true;
      digits.clear();
      digits.shrink_to_fit();
      digits.push_back(0);
      return *this;
    }
    sign = (sign == number.sign);
    if (number.digits.size() == 1) {
      int64_t add = 0;
      for (size_t i = 0; i < digits.size(); i++) {
        digits[i] = digits[i] * number.digits[0] + add;
        add = digits[i] / module;
        digits[i] %= module;
      }
      if (add != 0) {
        digits.push_back(add);
      }
      return *this;
    }
    std::vector<int> answer;
    answer.push_back(0);
    for (size_t i = 0; i < number.digits.size(); i++) {

      int64_t add = 0;
      for (size_t j = 0; j < digits.size(); j++) {
        if (i + j >= answer.size()) {
          answer.push_back(0);
        }
        int64_t result = answer[i + j] + number.digits[i] * digits[j] + add;
        answer[i + j] = result % module;
        add = result / module;
      }
      while (add != 0) {
        if (answer.size() <= digits.size() + i) {
          answer.push_back(0);
        }
        int64_t result = answer[digits.size() + i] + add;
        answer[digits.size() + i] = result % module;
        add = result / module;
      }
    }
    int64_t add = 0;
    size_t index = 0;
    digits.clear();
    digits.shrink_to_fit();

    while (index < answer.size() || add != 0) {
      if (index >= answer.size()) {
        answer.push_back(0);
      }
      add += answer[index];
      answer[index] = add % module;
      add /= module;
      digits.push_back(answer[index]);
      index++;
    }
    return *this;
  }

  BigInteger &operator/=(BigInteger number) {
    if (number.digits[number.digits.size() - 1] == 0) {
      digits.clear();
      digits.shrink_to_fit();
      sign = false;
      digits.push_back(1);
      return *this;
    }
    sign = (sign == number.sign);
    number.sign = true;
    bool check = false;
    std::vector<int> answer;
    BigInteger current;
    for (int i = static_cast<int>(digits.size() - 1); i > -1; i--) {
      current.right();
      current.digits[0] = digits[i];
      int left = 0;
      int right = module;
      while (right - left > 1) {
        int middle = (right + left) / 2;
        if (middle * number <= current) {
          left = middle;
        } else {
          right = middle;
        }
      }
      current -= (number * left);
      if (current == 0) {
        current.digits.clear();
        current.digits.shrink_to_fit();
      }
      if (left == 0 && check) {
        answer.push_back(left);
      } else if (left != 0) {
        answer.push_back(left);
        check = true;
      }
    }
    digits.clear();
    digits.shrink_to_fit();
    for (int i = answer.size() - 1; i > -1; i--) {
      digits.push_back(answer[i]);
    }
    if (digits.size() == 0) {
      digits.push_back(0);
    }
    if (digits[digits.size() - 1] == 0) {
      sign = true;
    }
    return *this;
  }

  BigInteger operator%=(const BigInteger &number) {
    *this -= (*this / number) * number;
    return *this;
  }

  BigInteger &operator++() {
    if (!sign) {
      Decrease_1();
    } else {
      Increase_1();
    }
    return *this;
  }

  BigInteger &operator--() {
    if (!sign) {
      Increase_1();
    } else {
      Decrease_1();
    }
    return *this;
  }

  BigInteger operator++(int) {
    BigInteger copy(*this);
    if (!sign) {
      Decrease_1();
    } else {
      Increase_1();
    }
    return copy;
  }

  BigInteger operator--(int) {
    BigInteger copy(*this);
    if (!sign) {
      Increase_1();
    } else {
      Decrease_1();
    }
    return copy;
  }

  BigInteger ABS() const {
    BigInteger copy(*this);
    copy.sign = true;
    return copy;
  }
};

bool operator < (const BigInteger &a, const BigInteger &b) {
  if (!a.sign && b.sign) {
    return true;
  }
  if (a.sign && !b.sign) {
    return false;
  }
  bool check = !(a.sign && b.sign);
  if (a.digits.size() < b.digits.size()) {
    check^=1;
    return check;
  } else if (a.digits.size() > b.digits.size()) {
    return check;
  } else {
    for (int i = static_cast<int>(a.digits.size()) - 1; i > -1; i--) {
      if (a.digits[i] > b.digits[i]) {
        return check;
      }
      if (b.digits[i] > a.digits[i]) {
        check ^= 1;
        return check;
      }
    }
  }
  return false;
}

bool operator>(const BigInteger &a, const BigInteger &b) {
  return b < a;
}

bool operator<=(const BigInteger &a, const BigInteger &b) {
  return !(a > b);
}

bool operator>=(const BigInteger &a, const BigInteger &b) {
  return !(a < b);
}

bool operator==(const BigInteger &a, const BigInteger &b) {
  if (a.digits.size() != b.digits.size() || a.sign != b.sign) {
    return false;
  }
  std::string a_str = a.toString();
  std::string b_str = b.toString();
  return a_str == b_str;
}

bool operator!=(const BigInteger &a, const BigInteger &b) {
  return !(a == b);
}

BigInteger operator*(BigInteger a, const BigInteger &b) {
  a *= b;
  return a;
}

BigInteger operator+(BigInteger a, const BigInteger &b) {
  a += b;
  return a;
}

BigInteger operator-(BigInteger a, const BigInteger &b) {
  a -= b;
  return a;
}

BigInteger operator/(BigInteger a, const BigInteger &b) {
  a /= b;
  return a;
}

BigInteger operator%(BigInteger a, const BigInteger &b) {
  a %= b;
  return a;
}

std::ostream &operator<<(std::ostream &out, const BigInteger &number) {
  std::string number_str = number.toString();
  out << number_str;
  return out;
}

std::istream &operator>>(std::istream &in, BigInteger &number) {
  std::string input;
  in >> input;
  number.digits.clear();
  number.digits.shrink_to_fit();
  number.sign = true;
  number = BigInteger(input.c_str());
  return in;
}

BigInteger operator ""_bi(unsigned long long number) {
  std::string new_string = std::to_string(number);
  char *constructor = new char[new_string.size()];
  for (size_t i = 0; i < new_string.size(); i++) {
    constructor[i] = new_string[i];
  }
  BigInteger num(constructor, new_string.size());
  delete[] constructor;
  return num;
}

BigInteger operator ""_bi(const char *number, size_t count) {
  BigInteger result(number, count);
  return result;
}

BigInteger &true_gcd(BigInteger &a, BigInteger &b) {
  if (b == 0) {
    a = a.ABS();
    return a;
  }
  a %= b;
  return true_gcd(b, a);
}

BigInteger gcd(const BigInteger &a, const BigInteger &b) {
  BigInteger a_copy(a);
  BigInteger b_copy(b);
  return true_gcd(a_copy, b_copy);
}

class Rational {
 private:
  BigInteger numerator;
  BigInteger denominator;
  void Reduce() {
    BigInteger current = gcd(numerator, denominator);
    if (current != 1) {
      numerator /= current;
      denominator /= current;
    }
    if (denominator < 0) {
      numerator = -numerator;
      denominator = -denominator;
    }
  }
 public:
  int module = 1e9;
  friend bool operator<(const Rational &a, const Rational &b);
  friend bool operator==(const Rational &a, const Rational &b);
  Rational(const BigInteger &a, const BigInteger &b) {
    numerator = a.ABS();
    denominator = b.ABS();
    if (a == 0) {
      denominator = 1;
    } else {
      this->Reduce();
      if ((a < 0) != (b < 0)) {
        numerator = -numerator;
      }
    }
  }
  Rational(int a, int b) {
    numerator = abs(a);
    denominator = abs(b);
    if (a == 0) {
      denominator = 1;
    } else {
      this->Reduce();
      if ((a < 0) != (b < 0)) {
        numerator = -numerator;
      }
    }
  }
  Rational(int a) {
    numerator = a;
    denominator = 1;
  }
  Rational(const BigInteger &a) {
    numerator = a;
    denominator = 1;
  }
  Rational() {}
  Rational &operator+=(const Rational &number) {
    BigInteger new_numerator = number.numerator;
    if (denominator != number.denominator) {
      BigInteger current = denominator / gcd(denominator, number.denominator) * number.denominator;
      new_numerator *= current / number.denominator;
      numerator *= current / denominator;
      denominator = current;
    }
    numerator += new_numerator;
    this->Reduce();
    return *this;
  }

  Rational &operator-=(const Rational &number) {
    BigInteger new_numerator = number.numerator;
    if (denominator != number.denominator) {
      BigInteger current = denominator / gcd(denominator, number.denominator) * number.denominator;
      new_numerator *= current / number.denominator;
      numerator *= current / denominator;
      denominator = current;
    }
    numerator -= new_numerator;
    this->Reduce();
    return *this;
  }

  Rational &operator*=(const Rational &number) {
    numerator *= number.numerator;
    denominator *= number.denominator;
    this->Reduce();
    return *this;
  }

  Rational &operator/=(const Rational &number) {
    numerator *= number.denominator;
    denominator *= number.numerator;
    this->Reduce();
    return *this;
  }

  std::string toString() const {
    std::string answer = numerator.toString();
    if (denominator == 1) {
      return answer;
    }
    answer += '/' + denominator.toString();
    return answer;
  }

  std::string asDecimal(size_t precision) const {
    BigInteger numerator1(numerator);
    std::string result = "";
    if (numerator < 0) {
      numerator1 = -numerator1;
      result += '-';
    }
    BigInteger answer = numerator1 / denominator;
    result += answer.toString();
    if (precision == 0) {
      if (result == "-0") {
        return "0";
      }
      return result;
    }
    result += '.';
    size_t count = result.size();
    answer = numerator1 % denominator;
    for (size_t i = 0; i < precision; i++) {
      answer *= module;
      std::string add = (answer / denominator).toString();
      for (int j = 0; j < 9 - static_cast<int>(add.size()); j++) {
        result += '0';
      }
      result += add;
      answer %= denominator;
      if (result.size() - count > precision) {
        break;
      }
    }
    size_t limit = result.size() - count - precision;
    for (size_t i = 0; i < limit; i++) {
      result.pop_back();
    }
    return result;
  }

  explicit operator double() const {
    std::string answer = this->asDecimal(20);
    double result = std::stod(answer);
    return result;
  }

  Rational operator-() {
    Rational copy(*this);
    copy.numerator = -copy.numerator;
    return copy;
  }
};

Rational operator+(Rational a, const Rational &b) {
  a += b;
  return a;
}

Rational operator-(Rational a, const Rational &b) {
  a -= b;
  return a;
}

Rational operator*(Rational a, const Rational &b) {
  a *= b;
  return a;
}

Rational operator/(Rational a, const Rational &b) {
  a /= b;
  return a;
}

bool operator<(const Rational &a, const Rational &b) {
  BigInteger first = a.numerator;
  BigInteger second = b.numerator;
  if (a.denominator != b.denominator) {
    BigInteger current = a.denominator / gcd(a.denominator, b.denominator) * b.denominator;
    first *= current / a.denominator;
    second *= current / b.denominator;
  }
  return first < second;
}

bool operator>(const Rational &a, const Rational &b) {
  return b < a;
}

bool operator<=(const Rational &a, const Rational &b) {
  return !(a > b);
}

bool operator>=(const Rational &a, const Rational &b) {
  return !(a < b);
}

bool operator==(const Rational &a, const Rational &b) {
  return (a.numerator == b.numerator && a.denominator == b.denominator);
}

bool operator!=(const Rational &a, const Rational &b) {
  return !(a == b);
}
