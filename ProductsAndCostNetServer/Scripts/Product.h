#pragma once
#include "Money.h"

#define NOT_SET "not_set"

using namespace MoneyLogic;

namespace ProductsLogic
{
    class Product
    {

    private:
        
        char _name[30] = NOT_SET;
        Money _cost;
        int _count = 0;
    
    public:

        Product();
        Product(const char *, Money, int);
        Product(const Product&);
        friend ostream& operator<<(ostream&, const Product&);
        bool IsDefault() const;
        const char* GetName() const;
        Money GetCost() const;
        void SetCost(Money cost);
        
    };
}


