#pragma once

#include "Product.h"

#include <fstream>
#include <string>
#include <map>

using namespace std;

namespace ProductsLogic
{
    class ProductStore
    {
    private:

        fstream _database;
        string _databasePath;
        map<int, Product> _productsSet;
    
        class ConsoleUtility
        {
        public:
            static void ClearRdbufIfNeed();
            static Product* ReadProduct();
        };

        class EncryptionUtility
        {
        public:
            static const int EncryptionKey = 7;
            static char* Encrypt(char*, int);
            static char* Decrypt(char*, int);
        };
    
        int GenerateProductCode() const;

        void WriteProduct(pair<int, Product>&);
        void WriteProducts(map<int, Product>&);
        void ReadProducts(map<int, Product>&);
        
    public:
    
        ProductStore(const string& databasePath);
        void PrintAllProducts() const;
        void CreateProduct();
        void EditProduct();
        void TryRemoveProduct();
        bool ContainsProduct(const string& productName);
        Product FindProduct(const char* productName) const;
        
    };
}

