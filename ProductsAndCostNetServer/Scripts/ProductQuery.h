#pragma once
#include <list>
#include <sstream>
#include <vector>

#define QUERY_WORDS_DELIMITER '|'

#include "ProductStore.h"

using namespace std;

namespace ProductsLogic
{
    class ProductQuery
    {
    private:

        list<Product> _query;
        
    public:
    
        ProductQuery(stringstream& serializedQuery);
        void UpdateProductsInfo(const ProductStore& store) const;
        Money CalculateCost() const;
    };
    
}


