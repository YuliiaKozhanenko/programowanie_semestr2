#include "Controller.h"

#include <iostream>
#include <string>
#include <fstream>
#include<cstdlib>
#include <locale>
#include <random>
#include <cmath>
#include <algorithm>

using namespace std;

void Controller::run()
{
    int userCommand = -1;

    while (userCommand != 0)
    {
        cout << "\n\n";
        cout << "1. Create a list of products" << endl;
        cout << "2. Initialize the list" << endl;
        cout << "3. Show list" << endl;
        cout << "4. Add new product" << endl;
        cout << "5. Delete product" << endl;
        cout << "6. Sort products by price" << endl;
        cout << "7. Edit product from list" << endl;
        cout << "8. Create a list of products based on a file" << endl;
        cout << "9. Find product by name" << endl;
        cout << "0. Exit" << endl;
        cout << "Input command: ";
        cin >> userCommand;
        cout << "\n";
        switch (userCommand)
        {
        case 1:
            create();
            break;
        case 2:
            list.initialize();
            system("pause");
            system("cls");
            break;
        case 3:
            cout << list << endl;
            break;
        case 4:
            list.add();
            system("pause");
            system("cls");
            break;
        case 5:
            list.delOne();
            system("pause");
            system("cls");
            break;
        case 6:
            sort();
            system("pause");
            system("cls");
            break;
        case 7:
            list.edit();
            system("pause");
            system("cls");
            break;
        case 8:
            readFromFile();
            break;
        case 9:
            list.findByName();
            system("pause");
            system("cls");
            break;
        case 0:
            writeToFile();
            break;
        default:
            cout << "\nIncorrect command. Try again\n";
        }
    }
}

void Controller::sort()
{
    //vector<Product*> products;
    vector<shared_ptr<Product>> products;
    list.sortByPrice(products);

    cout << "----------------------------Wish List----------------------------" << endl;
    cout << list.getMadeBy() << "'s list:" << endl;
    /*for (auto product : products)
    {
        product->printProduct(cout);
    }*/
    for_each(
            products.begin(),
            products.end(),
            [](auto product)
            {
                product->printProduct(cout);
            }
    );
}

void Controller::create()
{
    cout << "Who makes this list? Write your name:";
    cin.ignore(32767, '\n');
    getline(cin, list.getMadeBy());
    list.delAll();
    list.create();
    system("pause");
    system("cls");
}

void Controller::readFromFile()
{
    ifstream inputFile("BaseOfFilms.txt");
    if (!inputFile) {
        cout << "Error opening file!" << endl;
    }
    inputFile >> list;

    inputFile.close();
}

void Controller::writeToFile()
{
    string line1 = "NewBaseOfFilms.txt";
    ofstream myfileTo(line1, ios::out);

    if (myfileTo.is_open())
    {
        myfileTo << list;
        myfileTo.close();
        cout << "The data is saved" << endl;
    }
    else
    {
        cout << "Can't find file for the Films" << endl;
    }
}