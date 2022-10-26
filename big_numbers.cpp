//
// Created by mihai145 on 26.10.2022.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class BigInt {
private:
    vector<int> digits;

    explicit BigInt(const vector<int>& digits) : digits{digits} {};

public:
    BigInt() = default;

    explicit BigInt(const string& s) {
        digits.resize((int)s.size(), 0);
        for(int i = (int)s.size() - 1; i >= 0; i--) {
            digits[s.size() - 1 - i] = s[i] - '0';
        }
    }

    BigInt operator+(const BigInt& other) const {
        vector<int> result(max((int)digits.size(), (int)other.digits.size()) + 1, 0);

        int carry = 0;
        for(int i = 0; i < max((int)digits.size(), (int)other.digits.size()) || carry > 0; i++) {
            int current_digit = carry;
            if(i < (int)digits.size()) {
                current_digit += digits[i];
            }
            if(i < (int)other.digits.size()) {
                current_digit += other.digits[i];
            }

            carry = current_digit / 10;
            current_digit %= 10;

            result[i] = current_digit;
        }

        while(result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }

        return BigInt{result};
    }

    BigInt operator-(const BigInt& other) const {
        vector<int> result((int)digits.size(), 0);

        int carry = 0;
        for(int i = 0; i < (int)digits.size(); i++) {
            int current_digit = digits[i] - carry;
            if(i < (int)other.digits.size()) {
                current_digit -= other.digits[i];
            }

            if(current_digit < 0) {
                carry = 1;
                current_digit += 10;
            } else {
                carry = 0;
            }

            result[i] = current_digit;
        }

        while(result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }

        return BigInt{result};
    }

    BigInt operator*(const BigInt& other) const {
        BigInt result(vector<int>{0});

        for(int i = 0; i < (int)other.digits.size(); i++) {
            vector<int> current_row;
            for(int j = 0; j < i; j++) { //offset the current row in the multiplication
                current_row.push_back(0);
            }

            int carry = 0;
            for(int j = 0; j < (int)digits.size() || carry > 0; j++) {
                int current_digit = carry;
                if(j < (int)digits.size()) {
                    current_digit += other.digits[i] * digits[j];
                }

                carry = current_digit / 10;
                current_digit %= 10;

                current_row.push_back(current_digit);
            }

            while(current_row.size() > 1 && current_row.back() == 0) {
                current_row.pop_back();
            }

            result = result + BigInt(current_row);
        }

        return result;
    }

    bool operator<(const BigInt& other) const {
        if(digits.size() < other.digits.size()) {
            return true;
        }
        if(digits.size() > other.digits.size()) {
            return false;
        }

        for(int i = 0; i < (int)digits.size(); i++) {
            if(digits[i] < other.digits[i]) {
                return true;
            }
            if(digits[i] > other.digits[i]) {
                return false;
            }
        }

        return false;
    }

    bool operator==(const BigInt& other) const {
        return digits == other.digits;
    }

    friend istream& operator>>(istream& is, BigInt& b) {
        string s; is >> s;
        b = BigInt(s);
        return is;
    }

    friend ostream& operator<<(ostream& os, BigInt& b) {
        for(int i = (int)b.digits.size() - 1; i >= 0; i--) {
            cout << b.digits[i];
        }
        return os;
    }
};

int main() {
    BigInt x, y;
    cin >> x >> y;

    BigInt s = x + y;
    cout << x << '+' << y << '=' << s << '\n';

    BigInt d = x - y;
    cout << x << '-' << y << '=' << d << '\n';

    BigInt p = x * y;
    cout << x << '*' << y << '=' << p << '\n';

    return 0;
}