#include "ProductStore.h"

namespace ProductsLogic
{
    void ProductStore::ConsoleUtility::ClearRdbufIfNeed()
    {
        if (cin.rdbuf()->in_avail() != 0) {

            cin.clear();
            cin.ignore(10, '\n');
        }
    }

    Product* ProductStore::ConsoleUtility::ReadProduct()
    {
        string nameBuffer;
        int countBuffer;
        float unitCostBuffer;
        
        int progress = 0;
        const int maxSteps = 3;
        
        while (progress != -1 && progress < maxSteps)
        {
            ClearRdbufIfNeed();
                
            if (progress < 1)
            {
                cout << "Enter name of the product: " << endl;
                
                getline(cin, nameBuffer);

                if (nameBuffer == "undo")
                {
                    progress = -1;
                    break;
                }
            
                progress++;
            }
            
            if (progress < 2)
            {
                cout << "Enter the cost of unit: " << endl;
                cin >> unitCostBuffer;

                if (!cin)
                {
                    cout << "Incorrect input!" << endl;
                    continue;
                }
                else if (unitCostBuffer < 0.0f)
                {
                    cout << "Cost can not be negative." << endl;
                    continue;
                }

                progress++;
            }
            
            if (progress < 3)
            {
                cout << "Enter product count: " << endl;
                cin >> countBuffer;

                if (!cin)
                {
                    cout << "Incorrect input!" << endl;
                    continue;
                }
                else if (countBuffer < 0)
                {
                    cout << "Count can not be negative." << endl;
                    continue;
                }

                progress++;
            }
        }

        ClearRdbufIfNeed();
            
        if (progress >= maxSteps)
        {
            return new Product(nameBuffer.c_str(), Money(unitCostBuffer, "USD"), countBuffer); 
        }
        else
            return nullptr;
    }

    char* ProductStore::EncryptionUtility::Encrypt(char* data, int size)
    {
        for (int i = 0; i < size; i++)
        {
            data[i] = static_cast<char>(static_cast<int>(data[i]) + EncryptionKey);
        }
        
        return data;
    }

    char* ProductStore::EncryptionUtility::Decrypt(char* data, int size)
    {
        for (int i = 0; i < size; i++)
        {
            data[i] = static_cast<char>(static_cast<int>(data[i]) - EncryptionKey);
        }

        return data;
    }

    int ProductStore::GenerateProductCode() const
    {
        srand(time(nullptr));
        int generatedCode = rand() % 900 + 100;

        for (pair<int, Product> codeProductPair : _productsSet)
        {
            if (codeProductPair.first == generatedCode)
            {
                generatedCode = GenerateProductCode();
                break;
            }
        }
        
        return generatedCode;
    }
    
    void ProductStore::WriteProduct(pair<int, Product> &codeProductPair)
    {
        const int PairSize = sizeof(pair<int, Product>);
        char* decryptedChars = reinterpret_cast<char*>(&codeProductPair);
        _database.write(EncryptionUtility::Encrypt(decryptedChars, PairSize), PairSize);
    }
    
    void ProductStore::WriteProducts(map<int, Product> &products)
    {
        const int PairSize = sizeof(pair<int, Product>);
        
        for (pair<int, Product> codeProductPair : products)
        {
            char* decryptedChars = reinterpret_cast<char*>(&codeProductPair);
            _database.write(EncryptionUtility::Encrypt(decryptedChars, PairSize), PairSize);
        }
    }

    void ProductStore::ReadProducts(map<int, Product> &products)
    {
        const int PairSize = sizeof(pair<int, Product>);
        char encryptedCharsBuffer[PairSize];
        char* encryptedChars = encryptedCharsBuffer;
        
        while (_database.read(encryptedChars, PairSize)) {
            
            pair<int, Product> codeProductPair =
                *(reinterpret_cast<pair<int, Product>*>(EncryptionUtility::Decrypt(encryptedChars, PairSize)));
            
            products.emplace(codeProductPair);
        }
    }

    ProductStore::ProductStore(const string& databasePath)
    {
        _databasePath = databasePath;
        _productsSet = map<int, Product>();
        
        _database.open(_databasePath, fstream::in | fstream::binary);

        if(_database.is_open())
        {
            ReadProducts(_productsSet);
            _database.close();
        }
        else
        {
            throw exception("Incorrect database path.");
        }
    }
    
    void ProductStore::PrintAllProducts() const
    {
        int i = 0;
        cout << "\nProducts: \n";

        for (pair<int, Product> codeProductPair : _productsSet)
        {
            cout << i << ". " << "Code: " << codeProductPair.first << " " << codeProductPair.second << endl;
            i++;
        }

        cout << endl;
    }

    void ProductStore::CreateProduct()
    {
        _database.open(_databasePath, fstream::out | fstream::app | fstream::binary);

        if (_database.is_open()) {
            
            Product* createdProduct = ConsoleUtility::ReadProduct();

            if (createdProduct != nullptr)
            {
                pair<int, Product> codeProductPair = pair<int, Product>(GenerateProductCode(), *createdProduct);

                _productsSet.emplace(codeProductPair);
                WriteProduct(codeProductPair);
                
                cout << "Product was successfully created." << endl;

                delete createdProduct;
                createdProduct = nullptr;
            }
            else
            {
                cout << "Creation canceled." << endl;
            }
            
            _database.close();
        }
        else
        {
            throw exception("Incorrect database path.");
        }
    }

    void ProductStore::EditProduct()
    {
        cout << "Enter code of the product: " << endl;
        int productCode;

        ConsoleUtility::ClearRdbufIfNeed();
            
        if (cin >> productCode)
        {
            const auto it = _productsSet.find(productCode);

            if (it != _productsSet.end())
            {
                Product* editableProduct = ConsoleUtility::ReadProduct();

                if (editableProduct != nullptr)
                {
                    it->second = Product(*editableProduct);

                    _database.open(_databasePath, fstream::out | fstream::trunc | fstream::binary);
                    
                    if (_database.is_open())
                    {
                        WriteProducts(_productsSet);
                        _database.close();

                        cout << "Product successfully updated." << endl;
                    }
                    else
                    {
                        throw exception("Incorrect database path.");
                    }
                }
                else
                {
                    cout << "Creation canceled." << endl;
                }
            }
            else
            {
                cout << "Product with this code don't exists." << endl;
            }
        }
        else
        {
            cout << "Incorrect input." << endl;
        }

        ConsoleUtility::ClearRdbufIfNeed();
    }

    void ProductStore::TryRemoveProduct()
    {
        cout << "Enter code of the product: " << endl;
        int productCode;

        ConsoleUtility::ClearRdbufIfNeed();
            
        if (cin >> productCode)
        {
            int removesCount = _productsSet.erase(productCode);

            if (removesCount > 0)
            {
                _database.open(_databasePath, fstream::out | fstream::trunc | fstream::binary);

                if (_database.is_open())
                {
                    WriteProducts(_productsSet);
                    _database.close();

                    cout << "Product successfully removed." << endl;
                }
                else
                {
                    throw exception("Incorrect database path.");
                }
            }
            else
            {
                cout << "Product with this code doesn't exist." << endl;
            }
        }
        else
        {
            cout << "Incorrect input." << endl;
        }

        ConsoleUtility::ClearRdbufIfNeed();
    }

    bool ProductStore::ContainsProduct(const string& productName)
    {
        const char* name = productName.c_str();
        
        for (const auto& pair : _productsSet)
        {
            if (strcmp(pair.second.GetName(), name) == 0)
            {
                return true;
            }
        }
        
        return false;
        
    }

    Product ProductStore::FindProduct(const char* productName) const
    {
        for (const auto& pair : _productsSet)
        {
            if (strcmp(pair.second.GetName(), productName) == 0)
            {
                return pair.second;
            }
        }
        
        return Product();
    }
}


