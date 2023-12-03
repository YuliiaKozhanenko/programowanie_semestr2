#include "WishList.h"
#include <iostream>
#include <string>
#include <algorithm>

// ������� ��������� ��� ���������� �� �����


// ������ ����������


class Controller
{
	WishList list;

public:
	Controller(const std::string& madeBy, int size) : list(madeBy, size) {}
	void run();
	void create();
	void sort();
	void readFromFile();
	void writeToFile();
};

