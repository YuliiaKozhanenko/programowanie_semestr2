#include "Game.h"
#include "Product.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>
#include <regex>
using namespace std;

void Game::printProduct(std::ostream& os) const
{
    if (typeid(os) == typeid(std::ofstream))
    {
        os << "@" << getProductType() << " ";   
    }
    Product::printProduct(os);
    os << endl;
}

std::string Game::getProductType() const
{
    return "Game";
}

int randomNum(int start, int end)
{
    random_device rd;
    default_random_engine dfe(rd());
    uniform_int_distribution<int>intDist(start, end);

    return intDist(dfe);
}

double randomNum(double start, double end)
{
    random_device rd;
    default_random_engine dfe(rd());
    uniform_real_distribution<double>intDist(start, end);

    return intDist(dfe);
}