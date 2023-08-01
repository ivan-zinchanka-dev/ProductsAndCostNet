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

        _query = list<Product>();
            
        for (int i = 0; i < queryNames.size(); i++)
        {
            Product deserializedProduct = Product(queryNames.at(i).c_str(), Default, stoi(queryCounts.at(i)));
            _query.emplace_back(deserializedProduct);
        }

        for (auto p : _query)
        {
            cout << p << endl;
        }
        
    }

    void ProductQuery::UpdateProductsInfo(const ProductStore& store) const
    {
        for (Product productInfo : _query)
        {
            Product foundProduct = store.FindProduct(productInfo.GetName());
            
            if (!foundProduct.IsDefault())
            {
                productInfo.SetCost(foundProduct.GetCost());
            }
        }
    }

    Money ProductQuery::CalculateCost() const
    {
        Money cost = Money(0.0f, "USD");

        for (const Product& productInfo : _query)
        {
            cost += productInfo.GetCost();
        }
        
        return cost;
    }
}

