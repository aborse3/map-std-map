#include<iostream>
#include<random>
#include <cassert>
#include<initializer_list>
using namespace std;

namespace cs540 {
std::uniform_int_distribution<int> distribution(0, 1);

int count = 0;

template<typename key_t, typename mapped_t>
struct node {
	int boundry = -1;
	std::pair<const key_t, mapped_t> *ValueType;
	struct node<key_t, mapped_t> *left = NULL;
	struct node<key_t, mapped_t> *right = NULL;
	struct node<key_t, mapped_t> *down = NULL;
	struct node<key_t, mapped_t> *up = NULL;
	node(const std::pair<const key_t, mapped_t> &v1) :
			ValueType(new std::pair<const key_t, mapped_t>(v1)) {
	}
	node() {
		ValueType = NULL;
	}
};

template<typename key_t, typename mapped_t>
class Map {
public:
	class Iterator;
	class ConstIterator;
	class ReverseIterator;
	node<key_t, mapped_t> *head;
	node<key_t, mapped_t> *tail;
	int n, h;
	std::default_random_engine generator;

	Map() {
		n = 0;
		h = 0;
		head = new node<key_t, mapped_t>(); //cout<<"allocated in Map constructor"<<endl;
		tail = new node<key_t, mapped_t>(); //cout<<"allocated in Map constructor"<<endl;
		head->boundry = 0;
		tail->boundry = 1;
		head->right = tail;
		tail->left = head;
	}
	~Map() {
		;
		destroy();
	}
	Map(const Map &m) {
		n = 0;
		h = 0;
		head = new node<key_t, mapped_t>(); //cout<<"allocated in Map constructor"<<endl;
		tail = new node<key_t, mapped_t>(); //cout<<"allocated in Map constructor"<<endl;
		head->boundry = 0;
		tail->boundry = 1;
		head->right = tail;
		tail->left = head;
		copy_map(&m, this);
	}

	Map &operator=(const Map &m) {
		if (this == &m)
			return *this;
		else
			copy_map(&m, this);
		return *this;
	}

	mapped_t &operator[](const key_t &k) {
		//std::pair< Iterator,bool> it1;
		node<key_t, mapped_t> *p = findEntry(k);
		if ((p->ValueType == NULL) || (!(p->ValueType->first == k))) {
			mapped_t m { };
			insert( { k, m });
			//cout<<"inserted"<<it1.first.pointing_to->ValueType->second<<endl;
			p = findEntry(k);
		}
		//cout<<p->ValueType->second<<endl;
		return p->ValueType->second;
	}

	Map(std::initializer_list<const std::pair<const key_t, mapped_t> > list1) {
		n = 0;
		h = 0;
		head = new node<key_t, mapped_t>(); //cout<<"allocated in Map constructor"<<endl;
		tail = new node<key_t, mapped_t>(); //cout<<"allocated in Map constructor"<<endl;
		head->boundry = 0;
		tail->boundry = 1;
		head->right = tail;
		tail->left = head;
		for (const std::pair<const key_t, mapped_t> *p = list1.begin();
				p < list1.end(); p++)
			insert(*p);
	}

	bool operator==(const Map &m2) {
		return check_equality(*this, m2);
	}

	bool operator!=(const Map &m2) {
		return !(check_equality(*this, m2));
	}

	bool operator<(const Map &m2) {
		node<key_t, mapped_t> *pt1;
		node<key_t, mapped_t> *pt2;
		pt1 = this->head;
		pt2 = m2.head;
		while (pt1->down != NULL) {
			pt1 = pt1->down;
		}
		while (pt2->down != NULL) {
			pt2 = pt2->down;
		}
		pt1 = pt1->right;
		pt2 = pt2->right;

		while (pt1->boundry != 1 || pt2->boundry != 1) {
			if (!(pt1->ValueType->first < pt2->ValueType->first)) {
				return false;
			} else if (!(pt1->ValueType->second < pt2->ValueType->second)) {
				return false;
			}
		}
		if (pt1->boundry == 1 && pt1->boundry == 1)
			return false;
		else if (pt1->boundry == 1)
			return false;
		else
			return true;
	}

	bool check_equality(const Map &m1, const Map &m2) {
		if (m1.n != m2.n) {
			return false;
		} else {
			node<key_t, mapped_t> *pt1;
			node<key_t, mapped_t> *pt2;
			pt1 = m1.head;
			pt2 = m2.head;
			while (pt1->down != NULL) {
				pt1 = pt1->down;
			}
			while (pt2->down != NULL) {
				pt2 = pt2->down;
			}
			pt1 = pt1->right;
			pt2 = pt2->right;
			while (pt1->boundry != 1 && pt2->boundry != 1) {
				if (!(pt1->ValueType->first == pt2->ValueType->first)
						|| !(pt1->ValueType->second == pt2->ValueType->second)) {
					return false;
				}
				pt1 = pt1->right;
				pt2 = pt2->right;
			}
			return true;
		}
	}

	void copy_map(const Map<key_t, mapped_t> *old_map,
			Map<key_t, mapped_t>* new_map) {
		node<key_t, mapped_t> *p;
		p = old_map->head;
		while (p->down != NULL)
			p = p->down;
		p = p->right;

		while (p->boundry != 1) {
			new_map->insert(*(p->ValueType));
			p = p->right;
		}
	}

	node<key_t, mapped_t>* findEntry(const key_t k) const {
		node<key_t, mapped_t> *p;
		p = head;
		//cout<<"element to be searched is "<<k<<endl;
		while (true) {
			while (p->right->boundry != 1
					&& (p->right->ValueType->first < k
							|| p->right->ValueType->first == k)) {
				p = p->right;
			}
			if (p->down != NULL) {
				p = p->down;
			} else {
				break;
			}
		}
		return p;
	}

	Iterator find(const key_t &k) {
		Iterator it1;
		node<key_t, mapped_t> *p = findEntry(k);
		if (k == p->ValueType->first) {
			it1.pointing_to = p;
			return it1;
		} else {
			return this->end();
		}
	}

	ConstIterator find(const key_t &k) const {
		ConstIterator it1;
		node<key_t, mapped_t> *p = findEntry(k);
		if (k == p->ValueType->first) {
			it1.pointing_to = p;
			return it1;
		} else {
			return this->end();
		}
	}

	std::pair<Iterator, bool> insert(
			const std::pair<const key_t, mapped_t> &v1) {
		node<key_t, mapped_t> *p, *q, *e, *f;
		std::pair<Iterator, bool> it1;
		int i;
		p = findEntry(v1.first);
		if (p->ValueType)
			//cout<<"find result is "<<p->ValueType->first<<endl;
			f = p;
		//cout<<"data of p "<<p->data<<endl;
		//if(p->boundry==0 || p->ValueType.first!=v1.first)
		if (p->boundry == 0 || !(p->ValueType->first == v1.first)) {
			//q=node<key_t,mapped_t>(v1);
			q = new node<key_t, mapped_t>(v1);
			//cout<<"allocated in insert"<<endl;
			count++;
			//q->ValueType.first=k;
			q->left = p;
			q->right = p->right;
			p->right->left = q;
			p->right = q;
			i = 0;
			it1.first.pointing_to = q;//making Iterator pointing to newly inserted Element
			it1.second = true;
			while (distribution(generator)) {
				if (i >= h) {
					node<key_t, mapped_t>* p1, *p2;
					h = h + 1;
					p1 = new node<key_t, mapped_t>();
					//cout<<"allocated in insert for new level"<<endl;
					p2 = new node<key_t, mapped_t>();
					//cout<<"allocated in insert for new level"<<endl;
					count++;
					count++;
					p1->boundry = 0;
					p2->boundry = 1;
					p1->right = p2;
					p1->down = head;
					p2->left = p1;
					p2->down = tail;
					head->up = p1;
					tail->up = p2;
					head = p1;
					tail = p2;
				}

				while (p->up == NULL) {
					p = p->left;
				}
				p = p->up;
				e = new node<key_t, mapped_t>(v1);
				//cout<<"allocated in insert for tower"<<endl;
				count++;
				//e->ValueType.first=k;
				e->left = p;
				e->right = p->right;
				e->down = q;

				p->right->left = e;
				p->right = e;
				q->up = e;
				q = e;
				i++;
			}
			n++;
		} else {
			it1.first.pointing_to = f;//making Iterator pointing to newly inserted Element
			it1.second = false;
		}
		return it1;
	}

	void destroy() {
		node<key_t, mapped_t> *p, *q;
		p = head;
		while (p != NULL) {
			if (p != NULL)
				q = p->down;
			destroyRow(p);
			p = q;
			//cout<<p->ValueType.first<<endl;
		}
	}

	void destroyRow(node<key_t, mapped_t> *q) {
		node<key_t, mapped_t> *r;
		while (q != NULL) {
			r = q;
			q = q->right;
			delete (r->ValueType);
			delete (r);
		}
	}

	void remove(key_t k) {
		node<key_t, mapped_t> *p, *q;
		//node<key_t,mapped_t> *q;
		p = findEntry(k);
		if (p->ValueType->first == k) {
			while (p != NULL) {
				p->left->right = p->right;
				p->right->left = p->left;
				//q=p;
				if (p->up == NULL) {
					delete (p->ValueType);
					delete (p);
					p = NULL;
				} else {
					p = p->up;
					delete (p->down->ValueType);
					delete (p->down);
				}

			}
			n--;
			//remove extra levels
			p = head;
			q = tail;
			//cout<<"sdfsfds"<<(p->right->ValueType.first)<<endl;
			while (p->right == q && q->left == p && h > 0) {
				//cout<<"idksfhg";
				head = head->down;
				tail = tail->down;
				head->up = NULL;
				tail->up = NULL;\
				delete (p->ValueType);
				delete (p);
				delete (q->ValueType);
				delete (q);
				p = head;
				q = tail;
				h--;
			}
		} else
			std::out_of_range("key not found");

	}

	Iterator begin() {
		node<key_t, mapped_t> *p;
		Iterator it1;
		p = head;
		while (p->down != NULL) {
			p = p->down;
		}
		p = p->right;
		it1.pointing_to = p;
		return it1;
	}

	Iterator end() {
		node<key_t, mapped_t> *p;
		Iterator it1;
		p = tail;
		while (p->down != NULL) {
			p = p->down;
		}
		it1.pointing_to = p;
		return it1;
	}

	ConstIterator begin() const {
		node<key_t, mapped_t>*p;
		ConstIterator it1;
		p = head;
		while (p->down != NULL) {
			p = p->down;
		}
		p = p->right;
		it1.pointing_to = p;
		return it1;
	}

	ConstIterator end() const {
		node<key_t, mapped_t>*p;
		ConstIterator it1;
		p = tail;
		while (p->down != NULL) {
			p = p->down;
		}
		it1.pointing_to = p;
		return it1;
	}

	ReverseIterator rbegin() {
		node<key_t, mapped_t>*p;
		ReverseIterator it1;
		p = tail;
		while (p->down != NULL) {
			p = p->down;
		}
		p = p->left;
		it1.pointing_to = p;
		return it1;

	}

	ReverseIterator rend() {
		node<key_t, mapped_t>*p;
		ReverseIterator it1;
		p = head;
		while (p->down != NULL) {
			p = p->down;
		}
		it1.pointing_to = p;
		return it1;
	}

	size_t size() const {
		return (size_t) n;
	}

	bool empty() const {
		if (n == 0)
			return true;
		return false;
	}

	void clear() {
		Iterator it1;
		Iterator it2;
		Iterator it3;
		it2 = end();
		for (it1 = begin(); it1.pointing_to != it2.pointing_to;) {
			it3 = it1;
			it3++;
			erase(it1);
			it1 = it3;
		}
	}

	void erase(Iterator pos) {
		remove(pos.pointing_to->ValueType->first);
	}

	void erase(const key_t &t) {
		remove(t);
	}

	mapped_t &at(const key_t &k) {
		node<key_t, mapped_t> *p;
		p = findEntry(k);
		if (p->ValueType->first == k)
			return p->ValueType->second;
		else
			throw out_of_range("Not found");
	}

	const mapped_t &at(const key_t &k) const {
		node<key_t, mapped_t> *p;
		p = findEntry(k);
		if (p->ValueType->first == k)
			return p->ValueType->second;
		else
			throw out_of_range("Not found");
	}

	template<typename IT_T>
	void insert(IT_T range_beg, IT_T range_end) {
		IT_T it_t1;
		for (it_t1 = range_beg; it_t1 != range_end; it_t1++) {
			insert(*(it_t1));
		}
	}
	//template <typename key_t,typename mapped_t>
	class Iterator {
	public:
		node<key_t, mapped_t> * pointing_to;

		Iterator & operator=(const Iterator &it) {
			if (this == &it) {
				return *this;
			}
			this->pointing_to = it.pointing_to;
			return *this;
		}

		Iterator &operator++() {
			pointing_to = pointing_to->right;
			return *(this);
		}

		Iterator operator++(int) {
			Iterator temp = *this;
			operator++();
			return temp;
		}

		Iterator &operator--() {
			pointing_to = pointing_to->left;
			return *(this);
		}

		Iterator operator--(int) {
			Iterator temp = *this;
			operator--();
			return temp;
		}

		pair<const key_t, mapped_t> &operator*() const {
			return *(pointing_to->ValueType);
		}

		pair<const key_t, mapped_t>* operator->() const {
			return (pointing_to->ValueType);
		}

		//template <typename key_t,typename mapped_t>
		bool operator==(const Iterator &it2) {
			//cout<<"inside1";
			return (this->pointing_to == it2.pointing_to);
		}

		bool operator==(const ConstIterator &it2) {
			return (this->pointing_to == it2.pointing_to);
		}

		bool operator==(const ReverseIterator &it2) {
			return (this->pointing_to == it2.pointing_to);
		}

		bool operator!=(const Iterator &it2) {
			return (this->pointing_to != it2.pointing_to);
		}

		bool operator!=(const ConstIterator &it2) {
			return (this->pointing_to != it2.pointing_to);
		}

		bool operator!=(const ReverseIterator &it2) {
			return (this->pointing_to != it2.pointing_to);
		}

	};

	//template <typename key_t,typename mapped_t>
	class ConstIterator {
	public:
		node<key_t, mapped_t> * pointing_to;

		ConstIterator & operator=(const ConstIterator &it) {
			if (this == &it) {
				return *this;
			}
			this->pointing_to = it.pointing_to;
			return *this;
		}

		ConstIterator &operator++() {
			pointing_to = pointing_to->right;
			return *(this);
		}

		ConstIterator operator++(int) {
			ConstIterator temp = *this;
			operator++();
			return temp;
		}

		ConstIterator &operator--() {
			pointing_to = pointing_to->left;
			return *(this);
		}

		ConstIterator operator--(int) {
			ConstIterator temp = *this;
			operator--();
			return temp;
		}

		const pair<const key_t, mapped_t> &operator*() const {
			return *(pointing_to->ValueType);
		}

		const pair<const key_t, mapped_t>* operator->() const {
			return (pointing_to->ValueType);
		}

		bool operator==(const ConstIterator &it2) {
			//cout<<"inside1";
			return (this->pointing_to == it2.pointing_to);
		}

		bool operator==(const Iterator &it2) {
			return (this->pointing_to == it2.pointing_to);
		}

		bool operator==(const ReverseIterator &it2) {
			return (this->pointing_to == it2.pointing_to);
		}

		bool operator!=(const ConstIterator &it2) {
			return (this->pointing_to != it2.pointing_to);
		}

		bool operator!=(const Iterator &it2) {
			return (this->pointing_to != it2.pointing_to);
		}

		bool operator!=(const ReverseIterator &it2) {
			return (this->pointing_to != it2.pointing_to);
		}

	};

	//template <typename key_t,typename mapped_t>
	class ReverseIterator {
	public:
		node<key_t, mapped_t> * pointing_to;

		ReverseIterator & operator=(const ReverseIterator &it) {
			if (this == &it) {
				return *this;
			}
			this->pointing_to = it.pointing_to;
			return *this;
		}

		ReverseIterator &operator--() {
			pointing_to = pointing_to->right;
			return *(this);
		}

		ReverseIterator operator--(int) {
			ReverseIterator temp = *this;
			operator--();
			return temp;
		}

		ReverseIterator &operator++() {
			pointing_to = pointing_to->left;
			return *(this);
		}

		ReverseIterator operator++(int) {
			ReverseIterator temp = *this;
			operator++();
			return temp;
		}

		pair<const key_t, mapped_t> &operator*() const {
			return *(pointing_to->ValueType);
		}

		pair<const key_t, mapped_t>* operator->() const {
			return (pointing_to->ValueType);
		}

		bool operator==(const ReverseIterator &it2) {
			//cout<<"inside1";
			return (this->pointing_to == it2.pointing_to);
		}

		bool operator==(const Iterator &it2) {
			return (this->pointing_to == it2.pointing_to);
		}

		bool operator==(const ConstIterator &it2) {
			return (this->pointing_to == it2.pointing_to);
		}

		bool operator!=(const ReverseIterator &it2) {
			return (this->pointing_to != it2.pointing_to);
		}

		bool operator!=(const Iterator &it2) {
			return (this->pointing_to != it2.pointing_to);
		}

		bool operator!=(const ConstIterator &it2) {
			return (this->pointing_to != it2.pointing_to);
		}

	};

};

}
