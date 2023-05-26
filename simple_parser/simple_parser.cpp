#include <iostream>
#include <algorithm>
#include "windows.h"
#include "parser_ini.h"



int main() {
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
	

	
	try {
		
			std::string query_variable = "";
			std::string source_string = "";
			std::string answer = "";
			std::cout << "Введите путь до входного файла ";
			std::cin >> source_string;
			parser_ini parser(source_string);
			while (true) {

			std::cout << "Введите запрашиваемую переменную в формате строки \"Section_номер_секции.имя_переменной\" ";
			std::cin >> query_variable;
			auto v = parser.get_value < std::string > (query_variable);
			std::cout << parser.get_asked_variable_name() + " = ";
			std::cout << v << std::endl;
			std::cout << std::endl;
				std::cout << "Продолжить поиск переменных?(y/n)";
				while (true) {
				std::cin >> answer;
				if (answer == "y" || answer == "n") break;
				else std::cout << "Введите корректный ответ ";
			}
				if (answer == "y") continue;
				else break;
		}
		
		}
	    catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	
	
	return 0;
}