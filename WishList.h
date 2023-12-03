#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Product.h"


struct CompareById
{
	bool operator()(const int& one, const int& two) const
	{
		return one < two;
	}
};

class WishList
{
	std::map<int, std::unique_ptr<Product>/*Product**/, CompareById> m_products;
	std::string m_made_by;
	size_t m_size;

public:

	WishList(std::string made_by, size_t size_of);
	~WishList();
	WishList(const WishList& other);

	void create();
	void initialize();
	void delAll();
	void add();
	void edit();
	void delOne();
	//void sortByPrice(std::vector<Product*>& products);
	void sortByPrice(std::vector<std::shared_ptr<Product>>& productsVector);
	void findByName();

	std::string& getMadeBy();

	WishList& operator=(const WishList& other);
	Product& operator[](int i);

	friend std::ostream& operator<<(std::ostream& os, WishList& list);
	friend std::istream& operator>>(std::istream& is, WishList& list);
};

std::ostream& operator<<(std::ostream& os, WishList& list);
std::istream& operator>>(std::istream& is, WishList& list);



