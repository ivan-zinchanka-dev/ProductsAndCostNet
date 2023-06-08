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

    ostream& operator<<(ostream& out, const Money &moneyObject) {

        out << setiosflags(ios::fixed) << setprecision(2) << moneyObject._count << " " << moneyObject._currency;
        return out;
    }

    istream& operator>>(istream& in, Money &moneyObject) {

        in >> moneyObject._count >> moneyObject._currency;
        return in;
    }

    Money& Money::operator=(const Money &moneyObject)
    {
        _count = moneyObject._count;
        strcpy_s(_currency, moneyObject._currency);

        return *this;
    }

    Money Money::operator+(const Money &moneyObject) {

        if (strcmp(this->_currency, moneyObject._currency) == 0) {
            
            return Money(this->_count + moneyObject._count, _currency);
        }
        else 
            return Default;
        
    }

    Money Money::operator-(const Money &object) {

        if (strcmp(this->_currency, object._currency) == 0) {
            
            return Money(this->_count - object._count, _currency);
        }
        else 
            return Default;
        
    }
    
    bool operator==(const Money &moneyLeft, const Money &moneyRight) {

        if (strcmp(moneyLeft._currency, moneyRight._currency) == 0) {
            
            return Money::ApproximatelyEquals(moneyLeft._count, moneyRight._count);
        }
        else
            return false;
    }

    template<typename T>
    Money Money::operator*(T &mult){
    
        return Money(_count * static_cast<float>(mult), this->_currency);
    }

    bool Money::ApproximatelyEquals(float left, float right)
    {
        const float eps = 0.00001f;
        return abs(left - right) < eps;
    }

}

