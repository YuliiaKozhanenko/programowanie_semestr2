#include "Game.h"
#include "Film.h"
#include "Product.h"
#include "WishList.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>
#include <regex>
#include <map>
#include <tuple>
#include <cctype>
#include <algorithm>
#include <functional>

using namespace std;

Product& WishList::operator[](int i)
{
    auto it = m_products.find(i);
    assert(it != m_products.end());
    return *(get<1>(*it)) /* *(it->second)*/; 
}


void WishList::create()
{
    m_size = randomNum(1, 12);

    for (int i = 0; i < m_size; ++i)
    {
        int type = randomNum(1, 2);
        if (type == 1)
        {
            //m_products.insert(make_pair(i, new Film));
            m_products.insert(make_pair(i, make_unique<Film>()));
        }
        else
        {
            //m_products.insert(make_pair(i, new Game));
            m_products.insert(make_pair(i, make_unique<Game>()));
        }
    }
}

void WishList::delAll()
{
    for_each(
        m_products.begin(),
        m_products.end(),
        [](auto& pair)
        {
            //delete get<1>(pair);
           get<1>(pair).reset();//âèäàëÿº îá'ºêòè, íà ÿê³ âêàçóº unique_ptr ³ âñòàíîâëþº âêàç³âíèê íà nullptr
        }
    );
    m_products.clear(); // Î÷èñòèòè map
}


WishList::WishList(string made_by = "Niewiadoma Pani", size_t size = 5)
    :m_made_by(made_by), m_size(size)
{
    cout << "The constructor of the WishList (" << this << ") class created a map of products" << endl;
    create();
}

WishList::~WishList()
{
    cout << "Destructor of the WishList(" << this << ") class deleted a map of products" << endl;
    delAll();
}

void WishList::initialize()
{
    for_each(
        m_products.begin(),
        m_products.end(),
        [](auto& productPair)
        {
            //Product* product = get<1>(productPair);
            auto& product = get<1>(productPair);

            if (Film* film = dynamic_cast<Film*>(product.get()))
            {
                film->setPrice(round(randomNum(20.01, 50.00) * 100) / 100);
                film->setName("Film#" + to_string(randomNum(1, 200)));
                film->setGenre("Genre#" + to_string(randomNum(1, 200)));
                film->generateRandomDate();
                film->setRecommendedBy("Person No" + to_string(randomNum(1, 21)));
            }
            else if (Game* game = dynamic_cast<Game*>(product.get()))
            {
                game->setPrice(round(randomNum(34.86, 250.00) * 100) / 100);
                game->setName("Game#" + to_string(randomNum(1, 200)));
                game->setGenre("Category#" + to_string(randomNum(1, 200)));
                game->generateRandomDate();
            }
        }
    );
}

WishList::WishList(const WishList& other)
{
    this->m_made_by = other.m_made_by;
    this->m_size = other.m_size;

    this->m_products.clear();
    for_each(
        other.m_products.begin(),
        other.m_products.end(),
        [this](const auto& productPair)
        {
            /*int id = get<0>(productPair);
            Product* otherProduct = get<1>(productPair);

            if (Film* otherFilm = dynamic_cast<Film*>(otherProduct))
            {
                Film* newFilm = new Film(*otherFilm);
                this->m_products.insert(make_pair(id, otherFilm));
            }
            else if (Game* otherGame = dynamic_cast<Game*>(otherProduct))
            {
                Game* newGame = new Game(*otherGame);
                this->m_products.insert(make_pair(id, otherGame));
            }*/
            int id = get<0>(productPair);
            const unique_ptr<Product>& otherProduct = get<1>(productPair);

            if (const Film* otherFilm = dynamic_cast<const Film*>(otherProduct.get()))
            {
                unique_ptr<Film> newFilm = make_unique<Film>(*otherFilm);
                this->m_products.insert(make_pair(id, move(newFilm)));
            }
            else if (const Game* otherGame = dynamic_cast<const Game*>(otherProduct.get()))
            {
                unique_ptr<Game> newGame = make_unique<Game>(*otherGame);
                this->m_products.insert(make_pair(id, move(newGame)));
            }

        }
    );
}


WishList& WishList::operator=(const WishList& other)
{
    if (&other != this)
    {
        this->WishList::~WishList();
        this->WishList::WishList(other);
    }
    return *this;
}


ostream& operator<<(ostream& output, WishList& list)
{
    if (list.m_size > 0)
    {
        if (typeid(output) == typeid(std::cout))
        {
            output << "----------------------------Wish List----------------------------" << endl;
            output << list.m_made_by << "'s list:" << endl;
            for_each(
                list.m_products.begin(),
                list.m_products.end(),
                [&](const auto& pair)
                {
                    get<1>(pair)->printProduct(output);
                }
            );
        }
        else if (typeid(output) == typeid(std::ofstream))
        {
            output << list.m_made_by << "'s list:" << endl;
            for_each(
                list.m_products.begin(),
                list.m_products.end(),
                [&](const auto& pair)
                {
                    get<1>(pair)->printProduct(output);
                }
            );
        }
        
    }
    else
        cout << "List does not exist" << endl;
    
    return output;
}

istream& operator>>(istream& input, WishList& list)
{
    list.delAll();
    string line;
    string filename = "BaseOfFilms.txt";
    ifstream myfile(filename);
    if (myfile.is_open())
    {
        int k = 1;
        int count = 0;
        for (int i = 0; i < 2; i++)
        {
            if (i == 0)
            {
                while (getline(myfile, line))
                    count++;
            }
            if (i == 1)
            {
                myfile.clear();
                myfile.seekg(0);

                count--;
                list.m_size = count;
                while (getline(myfile, line))
                {
                    if (k == 1)
                    {
                        int pos1 = line.find_first_of("'s");
                        list.getMadeBy() = line.substr(0, pos1);
                        k++;
                    }
                    else
                    {
                        regex pattern(R"(@(\w+)\s+id\s*<(\d+)>\s*\{([^}]+)\}\s*\(([\d.]+).+\)\s*(.+)\s+release:\s+(\d{1,2})\.(\d{1,2})\.(\d{4})(?:\s+\(recommended by\s+([^)]+)\))?)");

                        smatch matches;

                        if (regex_search(line, matches, pattern))
                        {
                            string productType = matches[1].str();
                            int id = stoi(matches[2].str());
                            string genre = matches[3].str();
                            string name = matches[5].str();
                            double price = stod(matches[4].str());
                            int day = stoi(matches[6].str());
                            int month = stoi(matches[7].str());
                            int year = stoi(matches[8].str());
                            string recommendedBy = matches[9].str();

                            // Ñòâîðåííÿ â³äïîâ³äíîãî òèïó ïðîäóêòó ç âèêîðèñòàííÿì îòðèìàíèõ äàíèõ
                            if (productType == "Film")
                            {
                                //Film* film = new Film;
                                unique_ptr<Film> film = make_unique<Film>();
                                film->setGenre(genre);
                                film->setName(name);
                                film->setPrice(price);
                                film->setDay(day);
                                film->setMonth(month);
                                film->setYear(year);
                                film->setId(id);
                                film->setRecommendedBy(recommendedBy);
                                //list.m_products.insert(make_pair(id, film));
                                list.m_products.insert(make_pair(id, move(film)));
                            }
                            else if (productType == "Game")
                            {
                                //Game* game = new Game;
                                unique_ptr<Game> game = make_unique<Game>();
                                game->setGenre(genre);
                                game->setName(name);
                                game->setPrice(price);
                                game->setDay(day);
                                game->setMonth(month);
                                game->setYear(year);
                                game->setId(id);
                                //list.m_products.insert(make_pair(id, game));
                                list.m_products.insert(make_pair(id, move(game)));
                            }
                            else
                            {
                                // ßêùî òèï ïðîäóêòó íå â³äïîâ³äàº "Film" àáî "Game" - ïîìèëêà
                                cout << "Invalid product type: " << productType << endl;
                            }
                        }
                    }
                }
            }

        }
        myfile.close();
        cout << list;
    }
    else
    {
        cout << "Can't find file with Films";
    }
    return input;
}

string& WishList::getMadeBy() { return m_made_by; }

void WishList::add()
{

    cout << "Add a game or a film? (Enter '1' or '2'): ";
    int choice;
    cin >> choice;
    cin.ignore(32767, '\n');
    string text = "Write like that - index (" + to_string(m_size) + "); name of genre; name of product; price; day.month.year";
    if (choice == 2)
    {
        text += "; who recommended";
    }

    cout << text << ": ";
    string input;
    getline(cin, input);

    int index;
    string genre, name;
    double price;
    int day, month, year;
    string recommendedBy;

    // Ðîçáèòòÿ ðÿäêà íà êîìïîíåíòè çà äîïîìîãîþ find òà substr
    int pos = input.find(';');
    try
    {
        index = stoi(input.substr(0, pos));
    }
    catch (invalid_argument)
    {
        cout << "Invalid input for index. Try again." << endl;
        return;
    }

    if (index != m_size)
    {
        cout << "Invalid input for index. Try again." << endl;
        return;
    }

    int prev_pos = pos + 1;
    pos = input.find(';', prev_pos);
    genre = input.substr(prev_pos, pos - prev_pos);

    prev_pos = pos + 1;
    pos = input.find(';', prev_pos);
    name = input.substr(prev_pos, pos - prev_pos);

    prev_pos = pos + 1;
    pos = input.find(';', prev_pos);
    try
    {
        price = stod(input.substr(prev_pos, pos - prev_pos));
    }
    catch (invalid_argument)
    {
        cout << "Invalid input for price. Try again." << endl;
        return;
    }

    prev_pos = pos + 1;
    pos = input.find('.', prev_pos);
    try
    {
        day = stoi(input.substr(prev_pos, pos - prev_pos));
    }
    catch (invalid_argument)
    {
        cout << "Invalid input for day. Try again." << endl;
        return;
    }

    prev_pos = pos + 1;
    pos = input.find('.', prev_pos);
    try
    {
        month = stoi(input.substr(prev_pos, pos - prev_pos));
    }
    catch (invalid_argument)
    {
        cout << "Invalid input for month. Try again." << endl;
        return;
    }

    prev_pos = pos + 1;
    pos = input.find(';', prev_pos);
    try
    {
        year = stoi(input.substr(prev_pos, pos - prev_pos));
        cout << input.substr(prev_pos, pos - prev_pos) << " OToTOTOàïððïàð" << endl;
    }
    catch (invalid_argument)
    {
        cout << input.substr(prev_pos, pos - prev_pos) << " OToTOTOTO" << endl;
        cout << "Invalid input for year. Try again." << endl;
        return;
    }

    if (choice == 2)
    {
        prev_pos = pos + 1;
        recommendedBy = input.substr(prev_pos);
    }

    // Ïåðåâ³ðêà äîïóñòèìîñò³ ââåäåíèõ äàíèõ
        if (index < 0 || index > m_size)
        {
            cout << "Invalid input for index. Try again." << endl;
            return;
        }

        if (day < 1 || day > 31)
        {
            cout << "Invalid input for day. Try again." << endl;
            return;
        }

        if (month < 1 || month > 12)
        {
            cout << "Invalid input for month. Try again." << endl;
            return;
        }

        // Ñòâîðåííÿ îá'ºêòà Film àáî Game ç³ ç÷èòàíèìè äàíèìè
        if (choice == 2)
        {
            /*Film* newFilm = new Film(name, price, genre, index, day, month, year, recommendedBy);
            m_products.insert(make_pair(index, newFilm));*/
            unique_ptr<Film> newFilm = make_unique<Film>(name, price, genre, index, day, month, year, recommendedBy);
            m_products.insert(make_pair(index, move(newFilm)));
        }
        else if (choice == 1)
        {
            /*Game* newGame = new Game(name, price, genre, index, day, month, year);
            m_products.insert(make_pair(index, newGame));*/
            unique_ptr<Game> newGame = make_unique<Game>(name, price, genre, index, day, month, year);
            m_products.insert(make_pair(index, move(newGame)));
        }
        else
        {
            cout << "Invalid choice. Product not added." << endl;
            return;
        }

        cout << "Product added successfully." << endl;

    m_size = m_size + 1;
    cout << "Product added successfully." << endl;
}



void WishList::delOne()
{
    int index;
    cout << "One product with index from 0 to " << m_size - 1 << " will be deleted: ";
    cin >> index;

    if (index < 0 || index >= m_size)
    {
        cout << "Invalid input for index. Try again." << endl;
        return;
    }

    // Âèäàëèòè âèáðàíèé ïðîäóêò
    auto it = m_products.find(index);
    if (it != m_products.end())
    {
        //delete get<1>(*it);//it->secound âèäàëåííÿ îá'ºêòó
        m_products.erase(it);//âèäàëåííÿ ïàðè êëþ÷-ïðîäóêò
    }
    // Âèäàëåííÿ óñ³õ ïðîäóêò³â ï³ñëÿ ³íäåêñó ³ çáåð³ãàííÿ ¿õ ó òèì÷àñîâèé êîíòåéíåð
    vector<int> elementsToDelete;
    for (auto it = m_products.begin(); it != m_products.end(); ++it)
    {
        if (get<0>(*it) > index)
        {
            elementsToDelete.push_back(get<0>(*it));
            get<1>(*it)->setId(get<1>(*it)->getId() - 1);//çìåíøóºìî id íà 1, áî ï³ñëÿ âèäàëåííÿ îäíîãî ïðîäóêòó âîíè çì³ùàþòüñÿ
        }
    }

    //map<int, Product*> temp;
    map<int, unique_ptr<Product>> temp;
    for (int element : elementsToDelete)
    {
        //temp.insert(make_pair(element - 1, m_products[element]));//äîäàºìî ïðîäóêòè äî òèì÷àñîâîãî ìàïó ç³ çì³íåíèìè êëþ÷àìè
        temp.insert(make_pair(element - 1, move(m_products[element])));
        m_products.erase(element);//âèäàëÿºìî åëåìåíòè ç íåàêòóàëüíèìè êëþ÷àìè
    }

    elementsToDelete.clear();

   // m_products.insert(temp.begin(), temp.end());
    m_products.insert(make_move_iterator(temp.begin()), make_move_iterator(temp.end()));//äîäàºìî åëåìåíòè ç àêòóàëüíèìè êëþ÷àìè
    temp.clear();

    m_size = m_size - 1;
    cout << m_size << " SIZE\n";

}


void WishList::edit()
{
    cout << "Enter the index of the product you want to edit (from 0 to " << m_size - 1 << "): " << endl;
    int index;
    cin >> index;
    cin.ignore(32767, '\n');

    auto it = m_products.find(index);

    if (it != m_products.end())
    {

        cout << "Element found: " << get<0>(*it) << " - ";
        m_products[index]->printProduct(cout);
        cout << endl;
    }
    else
    {
        cout << "Element not found, invalid input for index" << endl;
        return;
    }


    cout << "Edit the product details: " << endl;
    string text = "Write like that - name of genre; name of product; price; day.month.year";
    if (dynamic_cast<Film*>(m_products[index].get()))//àáî m_products[index]->productType()=="Film";
    {
        text += "; who recommended";
    }
    cout << text << ": ";
    string input;
    getline(cin, input);

    // Ç÷èòóâàííÿ äàíèõ ç ðÿäêà
    string genre, name;
    double price;
    int day, month, year;
    string recommendedBy;

    // Ðîçáèòòÿ ðÿäêà íà êîìïîíåíòè çà äîïîìîãîþ find òà substr

    int pos = input.find(';');
    genre = input.substr(0, pos);

    int prev_pos = pos + 1;
    pos = input.find(';', prev_pos);
    name = input.substr(prev_pos, pos - prev_pos);

    prev_pos = pos + 1;
    pos = input.find(';', prev_pos);
    try
    {
        price = stod(input.substr(prev_pos, pos - prev_pos));
    }
    catch (invalid_argument)
    {
        cout << "Invalid input for price. Try again." << endl;
        return;
    }

    prev_pos = pos + 1;
    pos = input.find('.', prev_pos);
    try
    {
        day = stoi(input.substr(prev_pos, pos - prev_pos));
    }
    catch (invalid_argument)
    {
        cout << "Invalid input for day. Try again." << endl;
        return;
    }

    prev_pos = pos + 1;
    pos = input.find('.', prev_pos);
    try
    {
        month = stoi(input.substr(prev_pos, pos - prev_pos));
    }
    catch (invalid_argument)
    {
        cout << "Invalid input for month. Try again." << endl;
        return;
    }

    if (day < 1 || day > 31)
    {
        cout << "Invalid input for day. Try again." << endl;
        return;
    }
    else if (month < 1 || month > 12)
    {
        cout << "Invalid input for month. Try again." << endl;
        return;
    }

    prev_pos = pos + 1;
    pos = input.find(';', prev_pos);
    try
    {
        year = stoi(input.substr(prev_pos, pos - prev_pos));
        cout << input.substr(prev_pos, pos - prev_pos) << " OToTOTOàïððïàð" << endl;
    }
    catch (invalid_argument)
    {
        cout << input.substr(prev_pos, pos - prev_pos) << " OToTOTOTO" << endl;
        cout << "Invalid input for year. Try again." << endl;
        return;
    }

    if (dynamic_cast<Film*>(m_products[index].get()))
    {
        prev_pos = pos + 1;
        recommendedBy = input.substr(prev_pos);
    }

    m_products[index]->setGenre(genre);
    m_products[index]->setName(name);
    m_products[index]->setPrice(price);
    m_products[index]->setDay(day);
    m_products[index]->setMonth(month);
    m_products[index]->setYear(year); 

    if (dynamic_cast<Film*>(m_products[index].get()))
    {
        Film* film = dynamic_cast<Film*>(m_products[index].get());
        film->setRecommendedBy(recommendedBy);
    }

    cout << "Product details updated successfully." << endl;
}

void WishList::sortByPrice(vector<shared_ptr<Product>>& productsVector)
{
    for_each(
        m_products.begin(),
        m_products.end(),
        [&productsVector](auto& pair)
        {
            productsVector.push_back(get<1>(pair).get()->clone());
            //productsVector.push_back(shared_ptr<Product>(get<1>(pair).get()->clone()));
        }
    );

    auto getPrice = mem_fn(&Product::getPrice);//ñòâîðåííÿ ôóíêòîðà getPrice (obiekt funkcyjny)
    //(ìîæå ïðèéìàòè îá'ºêò òèïó Product ³ âèêëèêàòè íà íüîìó ìåòîä getPrice, ïîâåðòàþ÷è ðåçóëüòàò öüîãî âèêëèêó)

    int direction;
    cout << "1. Low to high" << endl;
    cout << "0. High to low" << endl;
    cout << "Enter sorting direction: ";
    cin >> direction;

    if (direction == 1)
    {
        sort(
            productsVector.begin(),
            productsVector.end(),
            [&](const shared_ptr<Product>& a, const shared_ptr<Product>& b)
            {
                return bind(less<>(), bind(getPrice, a), bind(getPrice, b))();
            }
        );
    }
    else if (direction == 0)
    {
        sort(
            productsVector.begin(),
            productsVector.end(),
            [](const shared_ptr<Product>& a, const shared_ptr<Product>& b)
            {
                return a->getPrice() > b->getPrice();
            }
        );
    }
    else
    {
        cout << "Invalid input";
    }
}

string normalize(const string& str)
{
    string result;
    // Çàïèñóº          ñòð³íã      äî íîâîãî ñòð³íãó result ïîâí³ñòþ ç ìàëî¿ áóêâè
    transform(str.begin(), str.end(), back_inserter(result), ::tolower);//back_inserter ïðàóþº ÿê push_back(), àäàïòåð ³òåðàòîðà
    // Âèäàëåííÿ ïðîá³ë³â: remove ïåðåì³ùóº óñ³ ïðîá³ëè íà ê³íåöü òà ïîâåðòàº ³òåðàòîð íà ïåðøèé ïðîá³ë,
    //erase âèäàëÿº óñ³ ïðîá³ëè â³ä ïåðøîãî ³ äî ê³íöÿ
    result.erase(remove(result.begin(), result.end(), ' '), result.end());
    return result;
}

void WishList::findByName()
{
    cout << "Write name of product you are looking for: ";
    cin.ignore(32767, '\n');
    string productName;
    getline(cin, productName);

    auto it = find_if(
                    m_products.begin(),
                    m_products.end(),
                    [&productName](const auto& pair)/*const pair<int, Product*>& pair*/
                    {
                        return normalize(get<1>(pair)->getName()) == normalize(productName);
                    }
              );

    if (it != m_products.end())
    {
        cout << "Product found with id " << get<0>(*it) << endl;
        get<1>(*it)->printProduct(cout);
    }
    else
    {
        cout << "Product not found." << endl;
    }
}




