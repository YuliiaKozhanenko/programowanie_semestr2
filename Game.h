#include <iostream>
#include <string>
#include "Product.h"


using namespace std;

class Game : public Product
{
public:

	Game(std::string name = "NaMe Of G", double price = 100.00, std::string genre = "nAmE oF C", int id = 0, int day = 3, int month = 11, int year = 2019)
		: Product(name, price, genre, id, day, month, year)
	{

	}

	Game(const Game& other) : Product(other){}

	Game& operator=(const Product& other)
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
		}
		return *this;
	}
	~Game() override = default;
	std::string getProductType() const override;
	void printProduct(std::ostream& os) const override;

	std::shared_ptr<Product> clone() const override
	{
		return std::make_shared<Game>(*this);
	}
	/*Game* clone() const override
	{
		return new Game(*this);
	}*/
};



int randomNum(int start, int end);
double randomNum(double start, double end);