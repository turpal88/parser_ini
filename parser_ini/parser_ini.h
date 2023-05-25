#include <vector>
#include <fstream>
#include <stdexcept>
#include <string>
//#include <unordered_map>
#include <algorithm>

struct Section { //структура для секции. Секция состоит из названия ("тега") секции [Section_номер_секции]
	int number; //номер секции, указанный в файле для данной секции
	int row_number; //номер строки в файле где размещен тег секции
	
	//std::pair<std::string, std::string> variable_name_value; 
	std::vector <std::pair<std::string, std::string>> string_variables; //вектор пар имя_переменной, значение_переменной
	Section() {
		number = -1; //в конструкторе инициализируется номер 
		row_number = 0; //и строка в файле
	}
	void print_section() { //вспомогательный метод для печати содержимого секции
		std::cout << "Section" + std::to_string(number) << " в строке " + std::to_string(row_number) << std::endl;
		for (auto& x : string_variables) std::cout << x.first << " = " << x.second << std::endl;
		std::cout << std::endl;
	}

};


class parser_ini { //класс парсера
public:
	parser_ini(std::string source) :source(source) { //в конструкторе передается строка с путем до входного файла, который будет парситься
		section_name_template = "[Section]"; //инициализируется шаблон тега секции, чтобы сравнивать то что в файле с тем как должно быть
		row_numbers = 0; //количество строк в файле
		for (int i = 65; i < 91; i++) { //в этих циклах заполняется вектор int со значениями в десятичном формате допустимых символов для названий переменных. Взято из таблицы ASCII
			allowed_ascii_values_for_variable_name.push_back(i);
		}
		for (int i = 97; i < 123; i++) { //здесь буквы английского алфавита, заглавные и прописные, а также некоторые символы которые можно использовать в именах переменных
			allowed_ascii_values_for_variable_name.push_back(i);
		}
		for (int i = 48; i < 58; i++) {
			allowed_ascii_values_for_variable_name.push_back(i);
		}
		allowed_ascii_values_for_variable_name.push_back(95);
		

		


	}
	
	
	template<typename T>
	T get_value(const std::string& str) {
		//T value;
		int num = 0;
		std::string r = "";
		std::string name = "";
		if (str.find("Section") != std::string::npos) {
			if (str.find('.') != std::string::npos) {
				//if (str.find(']') == str.find('.') - 1) 
					int i = 8;
					while (i < str.find('.')) {
						r += (str[i] - '0');
						i++;
					}
				
				
			}
			else {
				std::string message = "Отсутсвует имя переменной в передаваемой методу строке. Должна быть \".\" после имени секции, затем имя переменной";
				throw std::invalid_argument(message);
			}
			
		}
		else {
			std::string message = "Ошибка в передаваемом методу имени секции!";
			throw std::invalid_argument(message);
		}
		num = std::stoi(r);
		size_t j = str.find('.');
		while (j < str.size() - 1) {
			j++;
			name += str[j];
		}
		parsing();
		T value;
		for (auto& n : section) {
			if (num == n.number) {
				for (auto& m : n.string_variables) {
					if (name == m.first) {
						//value = m.first;
						//return value;
						return static_cast<T>(m.second);
					}
					else {
						std::string message = "Запрашиваемая переменная найдена, но преобразовать ее к запрашиваемому типу невозможно.\n Доступные переменные из этой секции:\n";
						for (auto& x : n.string_variables) std::cout << x.first << std::endl;
						throw std::invalid_argument(message);
					}
				}
			}
			else {
				std::string message = "Секции с таким номером нет в файле.\n Доступные секции в файле:\n";
				for (auto& t : section) {
					std::cout << "Section" + t.number << std::endl;
				}
				for (auto& x : n.string_variables) std::cout << x.first << std::endl;
				throw std::invalid_argument(message);
			}
		}
	
		
		
	}
	
	/*
	Section* get_section(int i) {
		return &section[i];
	}
	size_t get_section_size() {
		return section.size();
	}
	*/

private:
	std::string section_name_template; //шаблон названия секции
	std::vector<int> allowed_ascii_values_for_variable_name; //вектор допустимых значений для имени переменной из ASCII-таблицы
	std::string source; //путь до файла
	std::string vector_for_parse_section_number; //для хранения номера секции
	int row_numbers; //строка в файле где располагается тег секции
	std::ifstream in; //входной поток
	std::vector<Section> section; //вектор с секциями

	//проверка что в секции нет задублированных по имени переменных
	void catch_unique_variables_in_section(Section& sec) {
		if (sec.string_variables.size() > 1) {
			for (int i = 0; i < sec.string_variables.size() - 1; i++) {
				for (int j = i + 1; j < sec.string_variables.size(); j++) {
					if (sec.string_variables[i].first == sec.string_variables[j].first) {
						std::string message = "В секции номер " + std::to_string(sec.number) + " в строке " + std::to_string(sec.row_number) + " имеются переменные с одинаковым именем! Исправьте входной файл";
						throw std::invalid_argument(message);
					}
				}
			}
		}
		
		
	}

	
	
	//проверка на одинаковые имена переменных в секциях
	void looking_for_equal_variable_names(std::vector<int>& vec) {
		
		
		std::vector<bool> is_variable_in_vec_1_unique; //вспом вектор, туда складываются индексы неуникальных по имени переменных в векторе с переменными string_variables
		
		//идем по циклу и смотрим массив vec попарно - всегда только 0 и 1 элемент, так как один из этой пары в конце метода будет удален, также как и соответствующий элемент в векторе string_variables
		while (vec.size() > 1) { //пока в векторе больше 1 элемента (есть что сравнивать)
			is_variable_in_vec_1_unique.clear(); //зачищаем вспом вектор
			for (int i = 0; i < section[vec[1]].string_variables.size(); i++) { //ставим для всех переменных в векторе string_variables что они уникальны
			is_variable_in_vec_1_unique.push_back(true);
		}
			if (section[vec[1]].string_variables.size() != 0) { //идем по циклу, сравнивая имена переменных
				for (int i = 0; i < section[vec[0]].string_variables.size(); i++) {
					for (int j = 0; j < section[vec[1]].string_variables.size(); j++) {
						if (is_variable_in_vec_1_unique[j] == true) { //сравнение будет выполняться только если переменная справа в паре не была отмечена как неуникальная
							if (section[vec[0]].string_variables[i].first == section[vec[1]].string_variables[j].first) { //если имена переменных равны
								section[vec[0]].string_variables[i].second = section[vec[1]].string_variables[j].second; //копируем значение переменной в паре справа налево
								is_variable_in_vec_1_unique[j] = false; //отмечаем что переменная неуникальна, чтобы дальше ее больше не смотреть в последующих циклах
								break; //при нахождении неуникальных имен дальше смотреть смысла нет - проверка на уникальность имен переменных в отдельной секции выполнялась ранее, все имена
								//переменных в секции уникальны, если встретили переменную один раз, больше она не попадется - выходим из внутреннего цикла
							}

						}
						
					}
				}

				//уникальные переменные копируем в элемент вектора string_variables с индексом vec[0]
				for (int i = 0; i < section[vec[1]].string_variables.size(); i++) {
					if (is_variable_in_vec_1_unique[i]) section[vec[0]].string_variables.push_back(section[vec[1]].string_variables[i]);
				}
				
				
			}
			section.erase(section.begin() + vec[1]); //удаляем полностью задублированную секцию
			vec.erase(vec.begin() + 1); //удаляем задублированный индекс из вектора индексов
			if (vec.size() > 1) {
				for (int i = 1; i < vec.size(); i++) vec[i]--; //если в этом векторе больше 1 элемента, уменьшаем оставшиеся элементы на 1 так как снесли одну секцию
			}
			
			
		}
	}

	//проверка на одинкаовые имена секций. из этого метода запускается следующий метод определения одинаковых имен переменных, но сначала определяем секции с одинаковыми номерами
	void looking_for_equal_section_numbers() {
		std::vector<int> equal_section_indexes_in_vector_section; //вектор сюда складываем индексы секций (индексы вектора section) которые имеют одинаковые имена
		bool is_element_visited = false; //флаг что элемент просмотрен (для пуша в вектор equal_section_indexes_in_vector_section)
		

		for (int i = 0; i < section.size() - 1; i++) { //идем по циклу и сравниваем номера секций
			equal_section_indexes_in_vector_section.clear(); //зачищаем вектор с неуникальными индексами секций
			for (int j = i + 1; j < section.size(); j++) {
				
					if (section[i].number == section[j].number) { //если номер секции равен номеру следующей по циклу
						if (!is_element_visited) { //если еще не пушили номер секции
							equal_section_indexes_in_vector_section.push_back(i); //запоминаем индекс секции - пуш в equal_section_indexes_in_vector_section
							is_element_visited = true; //взводим флаг
						}
						
						equal_section_indexes_in_vector_section.push_back(j); //также пушим индекс секции с неуникальным номером
						
					}
				
			}
			if (equal_section_indexes_in_vector_section.size() != 0) { //если найдена пара неуникальных по номеру секций и вектор с индексами не пуст
				looking_for_equal_variable_names(equal_section_indexes_in_vector_section); //запускаем метод поиска переменных с неуникальными именами - передаем туда метод с индексами
				is_element_visited = false; //в конце сбрасываем флаг перед следующей итерацией внешнего цикла
				
			}
			
			
		}
		
	}

	//распарсивание входного файла начинается отсюда - метод parsing()
	/*-------------------------------*/
	void parsing() {
		
		std::ifstream in; //создается переменная типа поток
		std::string p; //вспомогательная строка куда помещается содержимое текущей читаемой строки из файла для анализа
		in.open(source); //открываем файл
		if (in.is_open()) { //если он открылся

			Section parsed_section; //вспомогательный объект типа Section нужен чтобы его заполнить тегом секции (при условии что тег написан без ошибок), затем запушить этот объект в вектор класса парсера
			int digits_for_section_numbers[10] = { 0,1,2,3,4,5,6,7,8,9 }; //вспомогательный массив допустимых символов для номера секции
			bool is_section_parsed; //показывает что парсится тег секции (до его номера)
			bool is_section_number_parsed; //показывает что парсится номер секции
			bool is_comment_parsed; //показывает что парсится коммент. Коммент игнорируется
			bool is_variable_name_parsed; //показывает что парсится имя переменной
			bool is_variable_value_parsed; //показывает что парсится значение переменной
			bool wait_for_equal_mark; //показывает что ожидается знак "=" после окончания ввода имени переменной
			std::string variable_name; //вспомогательная переменная куда будет складываться посимвольно имя переменной
			std::string variable_value; //вспомогательная переменная куда будет складываться посимвольно значение переменной
			while (std::getline(in, p)) { //чтение файла в цикле построчно

				row_numbers++; //зашли в файл - строку ставиим в 1 (каждый цикл увеличиваем число)
				is_section_parsed = false; //обнуляем все вспомогательные булевы переменные
				is_section_number_parsed = false;
				is_comment_parsed = false;
				is_variable_name_parsed = false;
				is_variable_value_parsed = false;
				wait_for_equal_mark = false;
				variable_name = "";
				variable_value = "";


				for (size_t i = 0, j = 0; i < p.size(); i++) { //цикл по прочитанной строке

					//если не парсится тег секции, либо ее номер или значение переменной и при этом первый символ в строке не пробел, значит парсим имя переменной
					if (p[i] != ' ' && p[i] != '[' && p[i] != ';' && !is_section_parsed && !is_section_number_parsed && !is_variable_value_parsed) is_variable_name_parsed = true;
					//если первый символ - это "[" и при этом не парсится значение переменной, значит парсим тег секции
					if (p[i] == '[' && !is_section_parsed /* && !is_variable_name_parsed*/ && !is_variable_value_parsed) is_section_parsed = true;
					//если в строке символ ";" и при этом парсится не значение переменной то игнор строки
					if (p[i] == ';' /* && !is_section_parsed && !is_section_number_parsed*/ && !is_variable_value_parsed) break;
					//если парсится тег секции и прочитаны все символы до символа номера значит парсится номер секции
					if (is_section_parsed && j == 8) is_section_number_parsed = true;
					//если парсится тег секции до номера секции и при этом не парсится номер секции или имя переменной
					if (is_section_parsed && j < 8 && !is_variable_name_parsed && !is_section_number_parsed) {
						if (p[i] == section_name_template[j]) { //считываем каждый символ во вспомогательную переменную
							j++; continue;
						}
						else { //если хотя бы один символ до номер секции не соответствует шаблону, кидаем исключение
							std::string message = "Ошибка в теге секции! Строка номер " + std::to_string(row_numbers);
							std::cout << "Исключение номер 1 " << "\"" << p << "\"" << std::endl;
							throw std::invalid_argument(message);
						}
					}
					if (is_section_number_parsed && !is_variable_name_parsed) { //если парсится номер секции и не парсится имя переменной
						//проверяем символ номера на допустимость (что это цифры а не абы что)
						auto it = std::find(digits_for_section_numbers, digits_for_section_numbers + 10, p[i] - '0');
						//если символ - среди цифр от 0 до 9 то заполняем вспомогательную строку цифрами номера секции 
						if (it != digits_for_section_numbers + 10) vector_for_parse_section_number += p[i];
						else if (p[i] != ']') { //иначе кидаем исключение
							std::string message = "Ошибка в номере секции! Строка номер " + std::to_string(row_numbers);
							std::cout << "Исключение номер 2 " << "\"" << p << "\"" << std::endl;
							throw std::invalid_argument(message);
						}


						else if (p[i] == ']') { //если дошли до конца тега секции
							j += vector_for_parse_section_number.size(); //прибавляем ко вспомогательной переменной j кол-во символов номера секции
							if (j == 8) { //если номер вообще нет - 0 символов номера - кидаем исключение
								std::string message = "Отсутствует номер секции! Строка номер " + std::to_string(row_numbers);
								std::cout << "Исключение номер 3 " << "\"" << p << "\"" << std::endl;
								throw std::invalid_argument(message);
							}
							parsed_section.number = std::stoi(vector_for_parse_section_number); //записываем во всомогательный объект номер секции
							parsed_section.row_number = row_numbers; //и номер строки в которой находится секция в файле
							is_section_number_parsed = false; //обнуляем флаги парсинга номера секции
							is_section_parsed = false; //и парсинга тега секции
							section.push_back(parsed_section); //пушим вспомогательный объект в вектор класса
							parsed_section.number = -1; //"обнуляем" номер секции во вспомогательном объекте
							vector_for_parse_section_number = ""; //"обнуляем" вспомогательную переменную для номера секции
							for (size_t m = i + 1; m < p.size(); m++) { //в цикле проверяем что в теге секции после закрывающей скобки "]" нет каких-либо символов кроме пробела и ";"
								if (p[m] != ' ' && p[m] != ';') {//если такие есть - кидаем исключение
									std::string message = "Посторонние символы в строке секции! Строка номер " + std::to_string(row_numbers);
									std::cout << "Исключение номер 4 " << "\"" << p << "\"" << std::endl;
									throw std::invalid_argument(message);

								}

							}
						}

					}



					if (is_variable_name_parsed && j == 0 && p[i] != ' ') { //если парсится первый символ имени переменной
						std::vector<int>::const_iterator it; //итератор
						//проверка на допустимые символы для первого символа в имени переменной
						it = std::find(allowed_ascii_values_for_variable_name.begin(), allowed_ascii_values_for_variable_name.end(), static_cast<int>(p[i]));
						//допускаются латинские буквы, нижнее подчеркивание, в случае если первый символ - что-то другое, кидаем исключение
						if ((it == allowed_ascii_values_for_variable_name.end()) || ((p[i] == static_cast<char>(48)) || (p[i] == static_cast<char>(49)) || (p[i] == static_cast<char>(50))
							|| (p[i] == static_cast<char>(51)) || (p[i] == static_cast<char>(52)) || (p[i] == static_cast<char>(53)) || (p[i] == static_cast<char>(54))
							|| (p[i] == static_cast<char>(55)) || (p[i] == static_cast<char>(56)) || (p[i] == static_cast<char>(57)))) {
							std::string message = "Недопустимые символы в имени переменной! Строка номер " + std::to_string(row_numbers);
							std::cout << "Исключение номер 6 " << "\"" << p << "\"" << std::endl;
							throw std::invalid_argument(message);
						}
						else {
							variable_name += p[i]; //если все ок с первым символом, пушим его в переменную имени переменной
							j++; //увеличиваем счетчик, следующие символы имени переменной будут считаны в другой ветке if
						}



					}

					else if (is_variable_name_parsed && j > 0 && p[i] == '=') { //если парсится имя переменной и встретился знак "="
						is_variable_name_parsed = false; //сбрасываем флаг парсинга имени переменной
						is_variable_value_parsed = true; //взводим флаг парсинга значения переменной
						wait_for_equal_mark = false; //сбоасываем флаг ожидания знака "="

						//если добрались до конца строки, пушим имя переменной и пустое значение переменной в карту
						if (i == p.size() - 1) {
							section[section.size() - 1].string_variables.push_back(std::pair<std::string, std::string>(variable_name, variable_value));
							break;
						}  
						//в цикле игнорим все пробелы после знака "=", при встрече первого символа, который не "=" и не ";", будем далее парсить номер переменной с него,
						//приравнивая его индекс текущему итератору i
						else if (i < p.size() - 1) {
							for (size_t t = i + 1; i < p.size(); t++) {
								if (p[t] != ' ' && p[i] != ';') {
									i = t;
									break;
								}
							}
						}



					}
					else if (is_variable_name_parsed && j > 0 && p[i] != ' ' && !wait_for_equal_mark) { //если парсится имя переменной (со 2-го символа) и текущий символ не пробел и не ждем знак "="
						std::vector<int>::const_iterator it;//итератор
						//проверяем что символы имени переменной допустимы (латинские буквы, цифры, нижнее подчеркивание)
						it = std::find(allowed_ascii_values_for_variable_name.begin(), allowed_ascii_values_for_variable_name.end(), static_cast<int>(p[i]));
						if (it != allowed_ascii_values_for_variable_name.end()) variable_name += p[i];
						else { //если символ вне допустимого диапазона - кидаем исключение
							std::string message = "Недопустимые символы в имени переменной! Строка номер " + std::to_string(row_numbers);
							std::cout << "Исключение номер 7 " << "\"" << p << "\"" << std::endl;
							throw std::invalid_argument(message);
						}


					}
					//если при парсинге имени переменной встретился пробел, взводим флаг что дальше мы ждем знак "="
					else if (is_variable_name_parsed && j > 0 && p[i] == ' ') wait_for_equal_mark = true;

					//если парсится имя переменной (знак "=" не введен) и достигнут конец строки либо ждем знак "=" и при этом символ не является ни пробелом ни знаком "=" значит знака "=" нет
					//кидаем исключение в таком случае - либо в имени переменной прямо посреди имени стоит пробел либо не поставлен знак "="
					if ((is_variable_name_parsed && !is_variable_value_parsed && i == p.size() - 1) || (wait_for_equal_mark && p[i] != ' ' && p[i] != '=')) {
						std::string message = "Отсутствует знак \"=\" после имени переменной! Строка номер " + std::to_string(row_numbers);
						std::cout << "Исключение номер 8 " << "\"" << p << "\"" << std::endl;
						throw std::invalid_argument(message);
					}

					//если парсится значение переменной, символ не является ";" и не пробел, заполняем вспомогательную переменную значения переменной
					if (is_variable_value_parsed && p[i] != ';') variable_value += p[i];

					//если встретился символ ";" либо достигнут конец строки
					if (is_variable_value_parsed && (p[i] == ';' || i == p.size() - 1)) {
						//пушим имя переменной и ее значение в карту
						section[section.size() - 1].string_variables.push_back(std::pair<std::string, std::string>(variable_name, variable_value));
						break;

					}




				}


			}




		}
		else throw std::invalid_argument("Входной файл поврежден или отсутствует!"); //если файл не открывается - кидаем исключение
		in.close();
		/*-------------------------------*/
		//распарсивание входного файла заканчивается здесь

		
		/*
		std::cout << "До распарсивания:" << std::endl;
		for (int i = 0; i < get_section_size(); i++) {
			get_section(i)->print_section();
		}
		std::cout << std::endl;
		*/
		//std::cout << "/*------------------------------------*/";
	   // std::cout << std::endl;
		
		for (auto n : section) catch_unique_variables_in_section(n); //проверка на задублированные переменные, если дубли есть - кидаем исключение
		
		//сравниваем все секции по номерам, если номера совпадают - сравниваем все переменные секций с одинаковыми номерами попарно, беря каждую пару секций.
		//рассматривая пару секций на равенство имен переменных, отмечаем те переменные которые во второй секции в паре неуникальны, то есть имена таких переменных одинаковые в обеих секциях  в паре
		//затем значения неуникальных переменных копируются из секции стоящей правее (а значит ниже строчкой во входном файле) в секцию, стоящую левее в паре
		//уникальные переменные тупо копируются из правой секции пары в левую, затем правая секция пары удаляется из вектора
		looking_for_equal_section_numbers(); 

		/*
		std::cout << "После распарсивания:" << std::endl;
		for (int i = 0; i < get_section_size(); i++) {
			get_section(i)->print_section();
		}
			*/	
			
		
		
		/*
		std::vector<int> temp = looking_for_equal_section_numbers();
		for (auto n : temp) {
			std::cout << n << std::endl;
		}
		*/
	}

	
	
};