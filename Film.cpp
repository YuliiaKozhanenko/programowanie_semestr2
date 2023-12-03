#include "Game.h"
#include "Product.h"
#include "Film.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>
#include <regex>
using namespace std;

void Film::printProduct(std::ostream& os) const
{
    if (typeid(os) == typeid(std::cout))
    {
        Product::printProduct(os);
        cout << std::left << " (recommended by " << m_recommended_by << ")" << endl;
    }
    else if (typeid(os) == typeid(std::ofstream))
    {
        os << "@" << getProductType() << " ";
        Product::printProduct(os);
        os << std::left << " (recommended by " << m_recommended_by << ")" << endl;
    }
}

std::string Film::getProductType() const
{
    return "Film";
}

Film::Film(const Film& other) : Product(other)
{
    m_recommended_by = other.m_recommended_by;
}