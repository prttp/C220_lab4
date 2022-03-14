#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <list>

class human {
	std::string m_name;
	bool m_alive;
	std::shared_ptr<human> m_father;
	std::shared_ptr<human> m_mother;
	std::list<std::weak_ptr<human>> m_childs;
public:
	

	
	human(const std::string name, bool alive = true) :
		m_name{ name }, m_alive{ alive }{}

	
	void addFather(const std::shared_ptr<human>& f) {
		m_father = f;
	}

	
	void addMother(const std::shared_ptr<human>& m) {
		m_mother = m;
	}

	void addChild(const std::shared_ptr<human>& c) {
		if (c) {
			m_childs.push_back(std::weak_ptr<human>(c));
		}
	}


	void setDead() { m_alive = false; }

	
	static std::shared_ptr<human> child(std::shared_ptr<human> f, std::shared_ptr<human> m,	const std::string& name) {
		std::shared_ptr<human> ch{ new human{name} };
		ch->addFather(f);
		ch->addMother(m);
		if (f) {
			f->addChild(ch);
		}
		if (m) {
			m->addChild(ch);
		}
		return ch;
	}

	static void print(const std::shared_ptr<human>& s, size_t spaces = 0) {
		if (s == nullptr)
			return;

		for (size_t i = 0; i < spaces; ++i) std::cout << "\t";
		std::cout << "|| " << s->m_name << " : " << ((s->m_alive) ? "alive" : "dead") << std::endl;
		size_t sp = spaces;
		if (s->m_mother) {
			print(s->m_mother, ++spaces);
		}
		if (s->m_father) {
			print(s->m_father, ++sp);
		}
	}
		
};


