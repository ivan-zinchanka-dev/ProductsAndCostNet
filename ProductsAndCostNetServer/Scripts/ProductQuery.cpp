#include "ProductQuery.h"

namespace ProductsLogic
{
    ProductQuery::ProductQuery(stringstream& serializedQuery)
    {
        string queryWordBuffer;
        vector<string> queryNames;
        vector<string> queryCounts;

        bool toNames = true;
            
        while (getline(serializedQuery, queryWordBuffer, QUERY_WORDS_DELIMITER))
        {
            toNames ? queryNames.emplace_back(queryWordBuffer) : queryCounts.emplace_back(queryWordBuffer);
            toNames = !toNames;
        }

        _query = list<shared_ptr<Product>>();
            
        for (int i = 0; i < queryNames.size(); i++)
        {
            Product deserializedProduct = Product(queryNames.at(i).c_str(), Default, stoi(queryCounts.at(i)));
            _query.emplace_back(make_shared<Product>(deserializedProduct));
        }
    }

    void ProductQuery::UpdateProductsInfo(const ProductStore& store) const
    {
        for (shared_ptr<Product> productInfo : _query)
        {
            Product foundProduct = store.FindProduct((*productInfo).GetName());
            
            if (!foundProduct.IsDefault())
            {
                (*productInfo).SetUnitCost(foundProduct.GetUnitCost());
            }
        }
    }

    Money ProductQuery::CalculateCost() const
    {
        Money cost = Money(0.0f, "USD");

        for (shared_ptr<Product> productInfo : _query)
        {
            cost += (*productInfo).GetTotalCost();
        }
        
        return cost;
    }
}

