#include <iostream>
#include <string>
#include "Product.h"

class Film : public Product
{
private:

	std::string m_recommended_by;

public:

	Film(const Film& other);

	Film(std::string name = "NaMe Of F", double price = 45.00, std::string genre = "nAmE oF G", int id = 0, int day = 1, int month = 12, int year = 2015, std::string rec_by = "Me")
		: Product(name, price, genre, id, day, month, year), m_recommended_by(rec_by)
	{
	}
	~Film() {}

	std::string getRecommendedBy()const { return m_recommended_by; }
	void setRecommendedBy(std::string name) { m_recommended_by = name; }
	static int getCount() { return count; }
	std::string getProductType() const override;
	void printProduct(std::ostream& os) const override;

	Film& operator=(const Film& other)
	{
		if (this != &other)
		{
			this->m_name = other.getName();
			this->m_price = other.getPrice();
			this->m_genre = other.getGenre();
			this->m_id = other.getId();
			this->setDay(other.getDay());
			this->setMonth(other.getMonth());
			this->setYear(other.getYear());
			this->setRecommendedBy(other.getRecommendedBy());
		}
		return *this;
	}

	std::shared_ptr<Product> clone() const override
	{
		return std::make_shared<Film>(*this);
	}
	/*Film* clone() const override
	{
		return new Film(*this);
	}*/
};