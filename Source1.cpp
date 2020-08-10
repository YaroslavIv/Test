#include <Magick++.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;
using namespace Magick;

enum class Atribute
{
	none,
	load,		// <name> <filename>
	store,		// <name> <filename>
	blur,		// <from_name> <to_name> <size>
	resize,		// <from_name> <to_name> <new_width> <new_height>
	help,
	exit
};

class Comand
{
	vector<pair<Image, string>> file;
	Image img;
	
	string name;
	string filename;
	string from_name;
	string to_name;
	
	Atribute atribute;
	int size;
	int new_width;
	int new_height;

public:

	Comand() :atribute(Atribute::none), size(0), new_width(0), new_height(0) {}
	
	~Comand() { vector<pair<Image, string>>().swap(file); }

	void reception() {
		
		string __s;
		cin >> __s;

		if (__s == "load" or __s == "ld")
			atribute = Atribute::load;
		else if (__s == "store" or __s == "s")
			atribute = Atribute::store;
		else if (__s == "blur")
			atribute = Atribute::blur;
		else if (__s == "resize")
			atribute = Atribute::resize;
		else if (__s == "help" or __s == "h")
			atribute = Atribute::help;
		else if (__s == "exit" or __s == "quit" or __s == "q")
			atribute = Atribute::exit;
		else
			atribute = Atribute::none;
	}

	void load() {
		cin >> name;
		cin >> filename;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		img.read(filename);
		file.push_back({ img,name });
	}

	void store() {
		cin >> name;
		cin >> filename;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		find_if(begin(file), end(file), [this](auto i) {return i.second == this->name; })->first.write(filename);

	}

	void blur() {
		cin >> from_name;
		cin >> to_name;
		cin >> size;
		
		if (!cin)
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Повторите ввод еще раз!" << endl;
			return;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		file.push_back(*find_if(begin(file), end(file), [this](auto i) {return i.second == this->from_name; }));
		file[file.size() - 1].second = to_name;
		file[file.size() - 1].first.blur(size);
	}

	void resize() {
		cin >> from_name;
		cin >> to_name;
		cin >> new_width;
		cin >> new_height;
		
		
		if (!cin)
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Повторите ввод еще раз!" << endl;
			return;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		file.push_back(*find_if(begin(file), end(file), [this](auto i) {return i.second == this->from_name; }));
		file[file.size() - 1].second = to_name;
		file[file.size() - 1].first.resize(Geometry(new_width, new_height));
	}

	void help()
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << setw(15) << "Команды"			<< setw(50) << "параметры"																<< setw(45) << "действие"						<< endl
			 << setw(15) << "load, ld"			<< setw(70) << "<name> - имя изображения, <filename> -имя файла для загрузки"			<< setw(32) << "Загружает jpg картинку"			<< endl
			 << setw(15) << "store, s"			<< setw(70) << "<name>  <filename> -имя файла для сохранениея"							<< setw(32) << "Сохраняет jpg картинку"			<< endl
			 << setw(15) << "blur"				<< setw(70) << "<from_name> - имя <to_name> -имя <size> -размер сглаживаемой области"	<< setw(32) << "Сглаживане изображения"			<< endl
			 << setw(15) << "resize"			<< setw(70) << "<from_name> <to_name>  <new_width> - ширина <new_height> - высота"		<< setw(32) << "Изменение размера изображения"	<< endl
			 << setw(15) << "exit, quit, q"		<< setw(70) << "---------------------------------------------------------------"		<< setw(32) << "Выход"							<< endl << endl
			 << setw(10) << "Пример:"																																					<< endl
			 << setw(27) << "ld img1 image.jpg"																																			<< endl
			 << setw(27) << "blur img1 img1b 5"																																			<< endl
			 << setw(28) << "s img1b image2.jpg"																																		<< endl
			 << setw(11) << "q"																																							<< endl;
	}


	Atribute getAtribute()
	{
		return atribute;
	}
};

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");

	Comand comand;
	try
	{
		while (true)
		{
			comand.reception();

			switch (comand.getAtribute())
			{
			case Atribute::load:
				comand.load();
				break;
			case Atribute::store:
				comand.store();
				break;
			case Atribute::blur:
				comand.blur();
				break;
			case Atribute::resize:
				comand.resize();
				break;
			case Atribute::help:
				comand.help();
				break;
			case Atribute::exit:
				return 0;
				__fallthrough;
			default:
				break;
			}

		}
	}
	catch (Magick::Exception& er)
	{ cout << "Exception-" << er.what() << endl; }

}