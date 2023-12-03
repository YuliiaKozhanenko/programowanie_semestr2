#include "Product.h"

int Product::count = 0;


void Product::printProduct(std::ostream& os) const
{
	std::stringstream price_stream;
	price_stream << std::fixed << std::setprecision(2) << m_price;
	std::string formatted_price = "(" + price_stream.str() + " zl) ";
	os << "id " << std::setw(4) << std::left << "<" + std::to_string(m_id) + ">" << std::setw(25) << std::left << " {" + m_genre + "}"
		<< std::setw(6) << std::right << formatted_price << std::setw(20) << std::right << m_name << std::right << std::setw(16) << "release: " 
        << std::setfill('0') << std::setw(2) << releaseDate.m_day << "." << std::setw(2) << releaseDate.m_month << "." << releaseDate.m_year;
    os << std::setfill(' ');
}

void Product::generateRandomDate()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> yearDist(2000, 2023);
    std::uniform_int_distribution<> dayDist(1, 31);
    std::uniform_int_distribution<> monthDist(1, 12);

    releaseDate.m_month = monthDist(gen);
    releaseDate.m_year = yearDist(gen);

    // √енеруЇмо день залежно в≥д обраного м≥с€ц€
    if (releaseDate.m_month == 2) // Ћютий
    {
        std::uniform_int_distribution<> dayFebDist(1, 28);
        releaseDate.m_day = dayFebDist(gen);
    }
    else if (releaseDate.m_month == 4 || releaseDate.m_month == 6 || releaseDate.m_month == 9 || releaseDate.m_month == 11)
    {
        //  в≥тень, червень, вересень, листопад
        std::uniform_int_distribution<> day30Dist(1, 30);
        releaseDate.m_day = day30Dist(gen);
    }
    else
    {
        // ≤нш≥ м≥с€ц≥
        releaseDate.m_day = dayDist(gen);
    }
}

Product::Product(const Product& other)
{
    //count = other.count;
    releaseDate = other.releaseDate;
    m_name = other.m_name;
    m_price = other.m_price;
    m_genre = other.m_genre;
    m_id = other.m_id;
}
