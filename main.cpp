#include "human.h"
#include <iterator>



int main()
{

	//////////////////////////////////////////////////////////////////////////////////////////////
	//Задание 1. unique_ptr
	{

		//1.а - обеспечьте корректное выполнение фрагмента
		{
			std::vector<std::string*> v = { new std::string("aa"), new std::string("bb"), new std::string("cc") };
			//Распечатайте все строки
			for (auto& i : v) {
				std::cout << *i << std::endl;
				delete i;
				i = nullptr;
			}
			v.clear();
			__asm nop
			//???
		} //???

		//1.b - модифицируйте задание 1.а:
		 //обеспечьте посредством std::unique_ptr: 
		 //эффективное заполнение (вспомните про разные способы формирования std::unique_ptr), 
		 //безопасное хранение указателей на динамически создаваемые объекты std::string, 
		 //манипулирование,
		 //и освобождение ресурсов
		 //

		{
			//Распечатайте все строки
			std::vector<std::unique_ptr<std::string>> v;

			v.push_back(std::make_unique<std::string>("aa"));
			v.push_back(std::make_unique<std::string>("bb"));
			v.push_back(std::make_unique<std::string>("cc"));
			for (const auto& i : v) {
				std::cout << *i << std::endl;
			}
			__asm nop
			//??? Уничтожение динамически созданных объектов?
		} //???

		{//1.c - дополните задание 1.b добавьте возможность изменять хранящиеся строки
		 //следующим образом (например, добавить указанный суффикс: "AAA" -> "AAA_1")  
			std::vector<std::unique_ptr<std::string>> v;

			v.push_back(std::make_unique<std::string>("aa"));
			v.push_back(std::make_unique<std::string>("bb"));
			v.push_back(std::make_unique<std::string>("cc"));
			for (const auto& i : v) {
				std::cout << *i << std::endl;
			}
			for (const auto& i : v) {
				*i += "_1";
			}
			for (const auto& i : v) {
				std::cout << *i << std::endl;
			}

			__asm nop
		}

		{//1.d - динамический массив объектов	

		 //Создайте unique_ptr, который является оберткой для динамического массива
		 //с элементами std::string
		 //С помощью unique_ptr::operator[] заполните обернутый массив значениями
		 //Когда происходит освобождения памяти?
			size_t n = 5;
			auto v = std::make_unique<std::string[]>(n);
			for (size_t i = 0; i < n; i++) {
				v[i] = { 'a', static_cast<char>('0' + i) };
				std::cout << v[i] << std::endl;
			}
			__asm nop
		} // здесь

		{//1.e - массивы динамических объектов и пользовательская delete-функция (функтор)
		 //Задан стековый массив указателей на динамически созданные объекты
		 //Создайте unique_ptr для такого массива
		 //Реализуйте пользовательскую delete-функцию (функтор) для корректного 
		 //освобождения памяти

			std::string* arStrPtr[] = { new std::string("aa"), new std::string("bb"), new std::string("cc") };
			auto v = [count = sizeof(arStrPtr) / sizeof(std::string*)](std::string* ar[]){

				for (int i = 0; i < count; i++) {
					delete ar[i];
					ar[i] = nullptr;
				}
			};
			std::unique_ptr<std::string*, decltype(v)> p(arStrPtr, v);
			int count = sizeof(arStrPtr) / sizeof(std::string*);
			for (int i = 0; i < count; i++) {
				std::cout << *(p.get()[i]) << std::endl;
			}
			__asm nop
		}

		{//1.f Создайте и заполните вектор, содержащий unique_ptr для указателей на std::string
			//Посредством алгоритма copy() скопируйте элементы вектора в пустой список с элементами 
			//того же типа
			//Подсказка: перемещающие итераторы и шаблон std::make_move_iterator
			std::vector<std::unique_ptr<std::string> > v;
			v.emplace_back(new std::string("abc"));
			v.emplace_back(new std::string("def"));
			v.emplace_back(new std::string("ghi"));
			std::list<std::unique_ptr<std::string> > l/*(v.size())*/;
			std::copy(std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()), std::inserter(l, l.end()));
			__asm nop


		}
	}
	////////////////////////////////////////////////////////////////////////////////
	//Задание 2.shared_ptr + пользовательская delete-функция 

	//Реализовать возможность записи в файл данных (строчек) из разных источников 
	//Так как все "писатели" будут по очереди записывать свои данные в один и тот же файл,
	//логично предоставить им возможность пользоваться одним и тем же объектом потока вывода =>
	//безопасной оберткой для такого указателя является shared_ptr
	//а. Первый владелец должен открыть/создать файл для записи
	//б. Все остальные писатели должны присоединиться к использованию
	//в. Последний владелец указателя должен закрыть файл

	//Подсказка: имитировать порядок записи можно с помощью функции rand()
	
	{
	 std::shared_ptr<FILE> file(fopen("test.txt", "w"), &fclose);
	//"писатели":
	//Создать writer1, writer2
		auto writer1 = [](const char* s, FILE* f) {
			fputs(s, f);
		};
		auto writer2 = [](const char* s, FILE* f) {
			fputs(s, f);
		};
	//заданное число итераций случайным образом позволяем одному из "писателей" записать в файл
	//свою строчку
		int n = 20;
		for (int i = 0; i < n; i++) {
			if (rand() % 10) {
				writer1("writer1 ", file.get());
			}
			else {
				writer2("writer2 ", file.get());
			}
		}
		writer1("\n", file.get());
	
	__asm nop
	}//закрытие файла???

	
	/***************************************************************/
	//Задание 3.
	{
		
			//Дан массив элементов типа string
			std::string strings[] = { "abc", "123", "qwerty", "#$%", "mmm", "345" };
			//До завершения фрагмента строки должны существовать в единственном экземпляре.
			//Требуется обеспечить манипулирование строками а) без копирования и б) без изменения порядка
			//элементов в массиве!
			auto del = [](std::string* p) {};
			{
			//В std::set "складываем" по алфавиту обертки для строк, которые содержат только буквы 
			auto cmp = [](const std::shared_ptr<std::string> a, const std::shared_ptr<std::string> b) { return *a > * b; };
			std::set<std::shared_ptr <std::string>, decltype(cmp)  > sStrings(cmp);
			for (size_t i = 0; i < (sizeof(strings) / (sizeof(std::string))); i++) {
				auto c = &strings[i].front();
				bool ok = true;
				while (ok) {

					if ((*c<'A' || *c>'z') || (*c > 'Z' && *c < 'a')) {
						ok = false;
						break;
					}
					if (c == &strings[i].back()) { break; }
					c++;
				}
				if (ok) { sStrings.insert(std::shared_ptr<std::string>(&strings[i],del)); }
				__asm nop
			}
		}
		__asm nop
		/******************************************************************************************/

		//В std::vector "складываем" обертки для строк, которые содержат только цифры 
		std::vector<std::shared_ptr < std::string>> vStrings;
		{
			for (size_t i = 0; i < (sizeof(strings) / (sizeof(std::string))); i++) {
				auto c = &strings[i].front();
				bool ok = true;
				while (ok) {

					if (*c< '0' || *c>'9') {
						ok = false;
						break;
					}
					if (c == &strings[i].back()) { break; }
					c++;
				}
				if (ok) { vStrings.push_back(std::shared_ptr<std::string>(&strings[i], del)); }
			}
		}
		//Выводим на экран
		for (const auto& i : vStrings) {
			std::cout << *i << std::endl;
		}
		//Находим сумму
		int sum = 0;
		for (const auto& i : vStrings) {
			sum += std::stoi(*i);
		}
		

		/******************************************************************************************/
		//сюда "складываем" обертки для строк, которые не содержат ни символов букв, ни символов цифр
		//и просто выводим
		std::vector<std::shared_ptr < std::string>> cStrings;
		for (size_t i = 0; i < (sizeof(strings) / (sizeof(std::string))); i++) {
			auto c = &strings[i].front();
			bool ok = true;
			while (ok) {

				if (std::isalnum(*c)) {
					ok = false;
					break;
				}
				if (c == &strings[i].back()) { break; }
				c++;
			}
			if (ok) { cStrings.push_back(std::shared_ptr<std::string>(&strings[i],del)); }
		}
		for (const auto& i : cStrings) {
			std::cout << *i << std::endl;
		}

		__asm nop
	}


	/******************************************************************************************/
	//Задание 4. 
	{
		//Дано:
		std::string ar[] = { "my","Hello", "World" };
		std::vector < std::shared_ptr<std::string>> v = { std::make_shared<std::string>("good"), std::make_shared<std::string>("bye") };
		size_t size = sizeof(ar) / sizeof(ar[0]);
		for (size_t i = 0; i < size; ++i)
		{
			v.push_back(std::shared_ptr<std::string>(&ar[i], [](std::string* ptr) {}));
		}


		//а) Требуется добавить в вектор обертки для элементов массива, НЕ копируя элементы массива! 
		//б) Отсортировать вектор по алфавиту и вывести на экран
		//в) Обеспечить корректное освобождение памяти
		std::sort(v.begin(), v.end(),
			[](const std::shared_ptr<std::string>& left, const std::shared_ptr<std::string>& right)
			{ return *left.get() < *right.get(); });

		auto it = v.begin();
		while (it != v.end())
		{
			std::cout << **it << " ";
			++it;
		}
		std::cout << std::endl;

		__asm nop
	}
	/***************************************************************/
		//Задание 5. shared_ptr и weak_ptr
		//Создаем генеалогическое дерево посредством класса human. В классе хранятся:
		//имя - string
		//возможно признак: жив или уже нет...
		//родители - shared_ptr (родители не всегда известны...)
		//дети - контейнер из weak_ptr (чтобы избежать циклических зависимостей)

		//Методы класса human:
		//конструктор - для инициализации имени и признака
		//конструктор копирования, оператор присваивания, move ???
		//статический метод child() - 
		//				должен создать создать и вернуть обертку для родившегося человека
		//				+ сформировать все связи ребенка с родителями и наоборот

		//Ввести возможность распечатать генеалогическое дерево для указанного индивидума

	{
		//История должна с кого-то начинаться => "Жили-были дед да баба, например, Адам и Ева"
		//(то есть на самом деле два деда и две бабы):
		std::shared_ptr<human> grandM1(new human("Eva"));
		std::shared_ptr<human> grandF1(new human("Adam"));
		std::shared_ptr<human> grandM2(new human("Irina"));
		std::shared_ptr<human> grandF2(new human("Anton"));

		//у них появились дети - child():

		auto Cain = human::child(grandF1, grandM1, "Cain");
		auto Abel = human::child(grandF1, grandM1, "Abel");
		auto Avan = human::child(grandF2, grandM2, "Avan");
		
		//а у детей в свою очередь свои дети:
		auto Enokh = human::child(Cain, Avan, "Enokh");
		//...

		

		Abel->setDead();
		human::print(Abel);
		std::cout << std::endl;
		human::print(Enokh);
		std::cout << std::endl;
		__asm nop
	}





	__asm nop

}
