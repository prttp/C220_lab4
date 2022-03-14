#include "human.h"
#include <iterator>



int main()
{

	//////////////////////////////////////////////////////////////////////////////////////////////
	//������� 1. unique_ptr
	{

		//1.� - ���������� ���������� ���������� ���������
		{
			std::vector<std::string*> v = { new std::string("aa"), new std::string("bb"), new std::string("cc") };
			//������������ ��� ������
			for (auto& i : v) {
				std::cout << *i << std::endl;
				delete i;
				i = nullptr;
			}
			v.clear();
			__asm nop
			//???
		} //???

		//1.b - ������������� ������� 1.�:
		 //���������� ����������� std::unique_ptr: 
		 //����������� ���������� (��������� ��� ������ ������� ������������ std::unique_ptr), 
		 //���������� �������� ���������� �� ����������� ����������� ������� std::string, 
		 //���������������,
		 //� ������������ ��������
		 //

		{
			//������������ ��� ������
			std::vector<std::unique_ptr<std::string>> v;

			v.push_back(std::make_unique<std::string>("aa"));
			v.push_back(std::make_unique<std::string>("bb"));
			v.push_back(std::make_unique<std::string>("cc"));
			for (const auto& i : v) {
				std::cout << *i << std::endl;
			}
			__asm nop
			//??? ����������� ����������� ��������� ��������?
		} //???

		{//1.c - ��������� ������� 1.b �������� ����������� �������� ���������� ������
		 //��������� ������� (��������, �������� ��������� �������: "AAA" -> "AAA_1")  
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

		{//1.d - ������������ ������ ��������	

		 //�������� unique_ptr, ������� �������� �������� ��� ������������� �������
		 //� ���������� std::string
		 //� ������� unique_ptr::operator[] ��������� ��������� ������ ����������
		 //����� ���������� ������������ ������?
			size_t n = 5;
			auto v = std::make_unique<std::string[]>(n);
			for (size_t i = 0; i < n; i++) {
				v[i] = { 'a', static_cast<char>('0' + i) };
				std::cout << v[i] << std::endl;
			}
			__asm nop
		} // �����

		{//1.e - ������� ������������ �������� � ���������������� delete-������� (�������)
		 //����� �������� ������ ���������� �� ����������� ��������� �������
		 //�������� unique_ptr ��� ������ �������
		 //���������� ���������������� delete-������� (�������) ��� ����������� 
		 //������������ ������

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

		{//1.f �������� � ��������� ������, ���������� unique_ptr ��� ���������� �� std::string
			//����������� ��������� copy() ���������� �������� ������� � ������ ������ � ���������� 
			//���� �� ����
			//���������: ������������ ��������� � ������ std::make_move_iterator
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
	//������� 2.shared_ptr + ���������������� delete-������� 

	//����������� ����������� ������ � ���� ������ (�������) �� ������ ���������� 
	//��� ��� ��� "��������" ����� �� ������� ���������� ���� ������ � ���� � ��� �� ����,
	//������� ������������ �� ����������� ������������ ����� � ��� �� �������� ������ ������ =>
	//���������� �������� ��� ������ ��������� �������� shared_ptr
	//�. ������ �������� ������ �������/������� ���� ��� ������
	//�. ��� ��������� �������� ������ �������������� � �������������
	//�. ��������� �������� ��������� ������ ������� ����

	//���������: ����������� ������� ������ ����� � ������� ������� rand()
	
	{
	 std::shared_ptr<FILE> file(fopen("test.txt", "w"), &fclose);
	//"��������":
	//������� writer1, writer2
		auto writer1 = [](const char* s, FILE* f) {
			fputs(s, f);
		};
		auto writer2 = [](const char* s, FILE* f) {
			fputs(s, f);
		};
	//�������� ����� �������� ��������� ������� ��������� ������ �� "���������" �������� � ����
	//���� �������
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
	}//�������� �����???

	
	/***************************************************************/
	//������� 3.
	{
		
			//��� ������ ��������� ���� string
			std::string strings[] = { "abc", "123", "qwerty", "#$%", "mmm", "345" };
			//�� ���������� ��������� ������ ������ ������������ � ������������ ����������.
			//��������� ���������� ��������������� �������� �) ��� ����������� � �) ��� ��������� �������
			//��������� � �������!
			auto del = [](std::string* p) {};
			{
			//� std::set "����������" �� �������� ������� ��� �����, ������� �������� ������ ����� 
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

		//� std::vector "����������" ������� ��� �����, ������� �������� ������ ����� 
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
		//������� �� �����
		for (const auto& i : vStrings) {
			std::cout << *i << std::endl;
		}
		//������� �����
		int sum = 0;
		for (const auto& i : vStrings) {
			sum += std::stoi(*i);
		}
		

		/******************************************************************************************/
		//���� "����������" ������� ��� �����, ������� �� �������� �� �������� ����, �� �������� ����
		//� ������ �������
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
	//������� 4. 
	{
		//����:
		std::string ar[] = { "my","Hello", "World" };
		std::vector < std::shared_ptr<std::string>> v = { std::make_shared<std::string>("good"), std::make_shared<std::string>("bye") };
		size_t size = sizeof(ar) / sizeof(ar[0]);
		for (size_t i = 0; i < size; ++i)
		{
			v.push_back(std::shared_ptr<std::string>(&ar[i], [](std::string* ptr) {}));
		}


		//�) ��������� �������� � ������ ������� ��� ��������� �������, �� ������� �������� �������! 
		//�) ������������� ������ �� �������� � ������� �� �����
		//�) ���������� ���������� ������������ ������
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
		//������� 5. shared_ptr � weak_ptr
		//������� ��������������� ������ ����������� ������ human. � ������ ��������:
		//��� - string
		//�������� �������: ��� ��� ��� ���...
		//�������� - shared_ptr (�������� �� ������ ��������...)
		//���� - ��������� �� weak_ptr (����� �������� ����������� ������������)

		//������ ������ human:
		//����������� - ��� ������������� ����� � ��������
		//����������� �����������, �������� ������������, move ???
		//����������� ����� child() - 
		//				������ ������� ������� � ������� ������� ��� ����������� ��������
		//				+ ������������ ��� ����� ������� � ���������� � ��������

		//������ ����������� ����������� ��������������� ������ ��� ���������� ����������

	{
		//������� ������ � ����-�� ���������� => "����-���� ��� �� ����, ��������, ���� � ���"
		//(�� ���� �� ����� ���� ��� ���� � ��� ����):
		std::shared_ptr<human> grandM1(new human("Eva"));
		std::shared_ptr<human> grandF1(new human("Adam"));
		std::shared_ptr<human> grandM2(new human("Irina"));
		std::shared_ptr<human> grandF2(new human("Anton"));

		//� ��� ��������� ���� - child():

		auto Cain = human::child(grandF1, grandM1, "Cain");
		auto Abel = human::child(grandF1, grandM1, "Abel");
		auto Avan = human::child(grandF2, grandM2, "Avan");
		
		//� � ����� � ���� ������� ���� ����:
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
