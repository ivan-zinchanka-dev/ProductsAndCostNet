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
            static const int EncryptionKey = 6;
            static char* Encrypt(char*);
            static char* Decrypt(char*);
        };
    
        int GenerateProductCode() const;

        void WriteProduct(pair<int, Product>&);
        void WriteProducts(map<int, Product>&);
        void ReadProducts(map<int, Product>&);
        
    public:
    
        ProductStore(string);
        void PrintAllProducts() const;
        void CreateProduct();
        void EditProduct();
        void TryRemoveProduct();
        
    };
}

