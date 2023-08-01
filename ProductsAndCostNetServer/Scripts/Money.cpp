#include "Money.h"

namespace MoneyLogic
{
    Money::Money() = default;

    Money::Money(float count, const char *currency) {

        this->_count = count;
        strcpy_s(this->_currency, currency);
    }

    Money::Money(const Money &other) {

        _count = other._count;
        strcpy_s(_currency, other._currency);
    }

    ostream& operator<<(ostream& out, const Money &money) {

        out << setiosflags(ios::fixed) << setprecision(2) << money._count << " " << money._currency;
        return out;
    }

    istream& operator>>(istream& in, Money &money) {

        in >> money._count >> money._currency;
        return in;
    }

    Money& Money::operator=(const Money &other)
    {
        _count = other._count;
        strcpy_s(_currency, other._currency);

        return *this;
    }

    Money Money::operator+(const Money &other) const
    {
        if (strcmp(this->_currency, other._currency) == 0) {
            
            return Money(this->_count + other._count, _currency);
        }
        else 
            return Default;
    }

    Money Money::operator-(const Money &other) const
    {
        if (strcmp(this->_currency, other._currency) == 0) {
            
            return Money(this->_count - other._count, _currency);
        }
        else 
            return Default;
    }

    Money& Money::operator+=(const Money& other)
    {
        return *this = *this + other;
    }

    bool operator==(const Money &left, const Money &right) {

        if (strcmp(left._currency, right._currency) == 0) {
            
            return Money::ApproximatelyEquals(left._count, right._count);
        }
        else
            return false;
    }

    Money Money::operator*(float multiplier) const
    {
        return Money(_count * multiplier, this->_currency);
    }

    bool Money::ApproximatelyEquals(float left, float right)
    {
        const float eps = 0.00001f;
        return abs(left - right) < eps;
    }

}
