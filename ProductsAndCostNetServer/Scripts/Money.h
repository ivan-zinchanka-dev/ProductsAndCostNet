#pragma once
#include <iostream>
#include <iomanip>

using namespace std;

namespace MoneyLogic
{
    class Money
    {
    private:

        float _count = -1.0f;
        char _currency[6] = "N/D";
    
    public:
        
        Money();
        Money(float, const char*);
        Money(const Money&);
        friend ostream& operator<<(ostream&, const Money&);
        friend istream& operator>>(istream&, Money&);
        Money& operator=(const Money&);
        Money operator+(const Money&);
        Money operator-(const Money&);
        friend bool operator==(const Money&, const Money&);
        template<typename T>
        Money operator*(T&);
        static bool ApproximatelyEquals(float, float);

    };

    static const Money Default {-1.0f, "N/D"};
    
}

