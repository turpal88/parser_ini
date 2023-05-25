#include <iostream>
#include <algorithm>
#include "windows.h"
#include "parser_ini.h"



int main() {
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
	
	
	
	

	

	
	
	
	try {
		parser_ini parser("d:/parser_ini/in.ini");
		auto v = parser.get_value<int>("Section2.var3");
		
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	
	
	return 0;
}