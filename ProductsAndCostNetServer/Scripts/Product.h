#pragma once
#include "Money.h"

using namespace MoneyLogic;

namespace ProductsLogic
{
    class Product
    {

    private:
    
        char _name[30] = "not_set";
        Money _cost;
        int _count = 0;
    
    public:

        Product();
        Product(const char *, Money, int);
        Product(const Product&);
        friend ostream& operator<<(ostream&, const Product&);
        
    };
}


