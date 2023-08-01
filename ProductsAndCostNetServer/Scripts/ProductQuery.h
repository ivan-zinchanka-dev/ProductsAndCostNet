#pragma once

#include "ProductStore.h"

#include <vector>
#include <list>
#include <sstream>

#define QUERY_WORDS_DELIMITER '|'

using namespace std;

namespace ProductsLogic
{
    class ProductQuery
    {
    private:

        list<shared_ptr<Product>> _query;
        
    public:
    
        ProductQuery(stringstream& serializedQuery);
        void UpdateProductsInfo(const ProductStore& store) const;
        Money CalculateCost() const;
    };
    
}


