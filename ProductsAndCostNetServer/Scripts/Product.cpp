#include "Product.h"

namespace ProductsLogic
{
    Product::Product() = default;

    Product::Product(const char * name, Money cost, int count)
    {
        strcpy_s(this->_name, name);
        this->_cost = cost;
        this->_count = count;
    }

    Product::Product(const Product &other)
    {
        strcpy_s(this->_name, other._name);
        this->_cost = other._cost;
        this->_count = other._count;
    }

    ostream& operator<<(ostream& out, const Product &product) {
    
        return out <<
            "Name: " << product._name <<
            "  Cost of unit: " << product._cost <<
                "  Count: " << product._count;
    }
}


