#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>


class Vector{
private:
    std::vector<int> big_number;
    void normalize() {
        for (int i = big_number.size() - 1; i >= 0; --i) {
            if (big_number[i] >= 10) {
                // Если находимся в начале, добавляем новый разряд
                if (i == 0) {
                    big_number.insert(big_number.begin(), 0);
                    ++i;
                }
                big_number[i - 1] += big_number[i] / 10;
                big_number[i] %= 10;
            }
        }
        while (big_number.size() > 1 && big_number[0] == 0) {
            big_number.erase(big_number.begin());
        }
    }    
public:
    Vector(){}
    Vector(size_t length) : big_number(length, 0) {}
    Vector(const std::vector<int>& elems) : big_number(elems) {
        normalize();
    }
    
    std::vector<int>::const_iterator begin() const {
        return big_number.cbegin();
    }
    
    std::vector<int>::const_iterator end() const {
         return big_number.cend();
    }
    
    int& operator[](int index) {
		size_t index1=index;
        if (index1 >= big_number.size()) {
            throw std::out_of_range("Index out of range");
        }
        return big_number[index];
    }
    
    const int& operator[](int index) const {
		size_t index1=index;
        if (index1 >= big_number.size()) {
            throw std::out_of_range("Index out of range");
        }
        return big_number[index];
    }
    
    static Vector stringToVector(const std::string& str) {
        Vector res;
        for (char c : str) {
            if (std::isdigit(static_cast<unsigned char>(c))) { 
                res.big_number.push_back(static_cast<int>(c - '0')); 
            }
        }
        return res;
    }
    
    std::string numberToText() const {
        std::string text;
        for (size_t i = 0; i < big_number.size(); i += 3) {
            int val = 0;
    
            for (size_t j = 0; j < 3 && (i + j) < big_number.size(); ++j) {
                val = val * 10 + big_number[i + j]; 
            }
            if (val >= 0 && val <= 255) {
                char c = static_cast<char>(val);
                text += c;
            }
        }
        return text;
    }
	
	void textToNumber(const char& c) {
        big_number.clear();
        int digit = static_cast<int>(c);
        while (digit > 0) {
            big_number.insert(big_number.begin(), digit % 10);
            digit /= 10;
        }
        if (big_number.empty()) {
            big_number.push_back(0);
        }
    }
    
    Vector operator*(const Vector& other) const {
        size_t maxSize = big_number.size() + other.big_number.size();
        Vector result(maxSize); 
    
        for (size_t i = 0; i < big_number.size(); ++i) {
            for (size_t j = 0; j < other.big_number.size(); ++j) {
                result.big_number[maxSize - 1 - (i + j)] += big_number[big_number.size() - 1 - i] * other.big_number[other.big_number.size() - 1 - j];
            }
        }
        result.normalize();
        return result;
    }
    
    Vector operator*(int n) const {
        if (n == 0) {
            return Vector({0, 0});
        }
        size_t lenght = big_number.size() + 1;
        Vector result(lenght);
        for (size_t i = 0; i < big_number.size(); ++i){
            result.big_number[result.big_number.size() - 1 - i] += big_number[big_number.size() - 1 - i] * n;
        }
        
        result.normalize();
        return result;
    }
    
    bool operator<(const Vector& other) const {
        if (big_number.size() != other.big_number.size())
            return big_number.size() < other.big_number.size();
        for (size_t i = 0; i !=big_number.size(); ++i) {
            if (big_number[i] != other.big_number[i])
                return big_number[i] < other.big_number[i];
        }
        return false;
    }

    bool operator==(const Vector& other) const {
        return big_number == other.big_number;
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }

    bool operator<=(const Vector& other) const {
        return *this < other || *this == other;
    }

    bool operator>(const Vector& other) const {
        return other < *this;
    }

    bool operator>=(const Vector& other) const {
        return !(*this < other);
    }
    
    bool isZero() const {
        return big_number.size() == 1 && big_number[0] == 0;
    }
    
    Vector& operator=(const Vector& other) {
        if (this != &other) { 
            big_number = other.big_number;
        }
        return *this; 
    }
    
    Vector operator-(const Vector& other) const {
        if (*this < other) {
            throw std::invalid_argument("Result would be negative.");
        }

        Vector result;
        result.big_number.resize(big_number.size(), 0);
        int borrow = 0;

        for (size_t i = 0; i < big_number.size(); ++i) {
            int sub = big_number[big_number.size() - 1 - i] - borrow;
            if (i < other.big_number.size()) {
                sub -= other.big_number[other.big_number.size() - 1 - i];
            }
            if (sub < 0) {
                sub += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.big_number[result.big_number.size() - 1 - i] = sub;
        }

        result.normalize();
        return result;
    }

    Vector operator+(const Vector& other) const {
        Vector result;
        size_t maxSize = std::max(big_number.size(), other.big_number.size());
        result.big_number.resize(maxSize + 1, 0); 
        
        int carry = 0;
        for (size_t i = 0; i < maxSize; ++i) {
            int sum = carry;
            if (i < big_number.size()) {
                sum += big_number[big_number.size() - 1 - i];
            }
            if (i < other.big_number.size()) {
                sum += other.big_number[other.big_number.size() - 1 - i];
            }
            result.big_number[result.big_number.size() - 1 - i] = sum % 10;
            carry = sum / 10;
        }

        if (carry > 0) {
            result.big_number[0] = carry;
        } else {
            result.normalize();
        }

        return result;
    }

    Vector operator/(const Vector& other) const {
        if (other.isZero()) {
            throw std::invalid_argument("Division by zero");
        }
        if (*this < other) {
            return Vector({0}); 
        }
    
        Vector result;
        Vector rem;
    
        for (size_t i = 0; i < big_number.size(); ++i) {
            rem.big_number.push_back(big_number[i]);
            rem.normalize();
    
            int digit = 0;
            while (rem >= other) {
                rem = rem - other;
                ++digit;
            }
    
            result.big_number.push_back(digit);
        }
    
        result.normalize();
        return result;
    }

    Vector operator%(const Vector& other) const {
        Vector divisionResult = *this / other; 
        return *this - (divisionResult * other); 
    }
    
    Vector operator++(int){
        big_number[big_number.size()-1]++;
        normalize();
        return *this;
    }
    
    static Vector PowBig_Number(const Vector& base, const Vector& exponent, const Vector& mod) {
        Vector res({0, 1}); 
        Vector base_mod = base % mod; 
        Vector exp = exponent;
    
        while (exp > Vector({0})) { 
            if (exp.big_number.back() % 2 == 1) { 
                res = (res * base_mod) % mod; 
            }
            base_mod = (base_mod * base_mod) % mod; 
            exp = exp / Vector({0, 2}); 
        }
        
        return res;
    }
    //алгоритм Евклида + проверка на взаимно простые
    static bool IsCoprime(const Vector& a, const Vector& b) {
		Vector x = a;
		Vector y = b;
		while (y != Vector({0, 0})) {
			Vector rem = x % y;
			x = y;
			y = rem;
		}
		return (x == Vector({0, 1})); 
	}

};

std::ostream& operator <<(std::ostream& out, const Vector& v){
        out << "[ ";
        for (int elem : v) {
            out << elem << " ";
        }
        out << "]";
        return out;
        }
#endif // RSA_H
