#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <string>
#include <random>

class Product
{
protected:

	class Date
	{
		int m_day;
		int m_month;
		int m_year;
		friend Product;

	public:
		Date(int day=01, int month = 01, int year = 2015): m_day(day), m_month(month), m_year(year){}
	};
	

	static int count;
	Date releaseDate;
	std::string m_name;
	double m_price;
	std::string m_genre;
	int m_id;

public:

	void generateRandomDate();

	Product& operator=(const Product& other)
	{
		if (this != &other)
		{
			this->m_name = other.m_name;
			this->m_price = other.m_price;
			this->m_genre = other.m_genre;
			this->m_id = other.m_id;
		}
		return *this;
	}

	Product(const Product& other);

	Product() : m_name(""), m_price(0.0), m_genre(""), m_id(0)
	{ 
		generateRandomDate();
		m_id = count;
		count++;
	}

	Product(std::string name, double price, std::string genre, int id, int day, int month, int year) 
		: m_name(name), m_price(price), m_genre(genre), m_id(id), releaseDate(day, month, year)
	{
		m_id = count;
		count++;
	}

	virtual ~Product() { count--; }

	std::string getName() const { return m_name; }
	double getPrice() const { return m_price; }
	std::string getGenre() const { return m_genre; }
	int getId() const { return m_id; }
	int getDay() const { return releaseDate.m_day; }
	int getMonth() const { return releaseDate.m_month; }
	int getYear() const { return releaseDate.m_year; }
	static int getCount() { return count; }

	void setName(std::string name) { m_name = name; }
	void setPrice(double price) { m_price = price; }
	void setId(int id) { m_id = id; }
	void setGenre(std::string genre) { m_genre = genre; }
	void setDay(int day) { releaseDate.m_day = day; }
	void setMonth(int month) { releaseDate.m_month = month; }
	void setYear(int year) { releaseDate.m_year = year; }

	virtual void printProduct(std::ostream& os) const;
	virtual std::string getProductType() const = 0;
	virtual std::shared_ptr<Product> clone() const = 0;
	//virtual Product* clone() const = 0;
};
