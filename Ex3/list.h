#pragma once
#include <iostream>
using namespace std;

namespace Election {

	template <class T>
	class list
	{
	private:
		class NodeListTem
		{
		public:
			T data;
			NodeListTem* next;
			NodeListTem* prev;
		};

		NodeListTem* Head;
		NodeListTem* Tail;
		int _size;

	public:
		class Iterator {
			friend class list;
			friend class const_Iterator;
			NodeListTem* ptr;
		public:
			Iterator() {};
			Iterator(NodeListTem* _ptr) { ptr = _ptr; }
			Iterator(const Iterator& other) : ptr(other.ptr) {}
			Iterator& operator++() {
				ptr = ptr->next;
				return *this;
			}
			Iterator& operator++(int) {
				Iterator temp(this->ptr);
				ptr = ptr->next;
				return temp;
			}

			bool operator !=(const Iterator& other) const {
				return !(*this == other);
			}

			bool operator ==(const Iterator& other) const {
				return (this->ptr == other.ptr); // 
			}

			T& operator*() { return ptr->data; }
		};

		class const_Iterator {
			friend class list;
			const NodeListTem* ptr;
		public:

			const_Iterator(const NodeListTem* _ptr = nullptr) { ptr = _ptr; }
			const_Iterator(const Iterator* other) { this = other; }

			const const_Iterator& operator++() {
				ptr = ptr->next;
				return *this;
			}
			const const_Iterator& operator++(int) {
				const_Iterator temp(this->ptr);
				ptr = ptr->next;
				return temp;
			}

			bool operator !=(const const_Iterator& other) const {
				return !(*this == other);
			}

			bool operator ==(const const_Iterator& other) const {
				return (this->ptr == other.ptr); // 
			}
			void operator =(const Iterator* other) {
				ptr = other.ptr;
			}

			const T& operator*() const { return ptr->data; }
		};


		list() :Head(nullptr), Tail(nullptr), _size(0) {}
		~list() {
			NodeListTem* temp = Head;
			NodeListTem* del;
			while (temp != nullptr) {
				del = temp;
				temp = temp->next;
				delete del;
			}
		}

		void push_front(T _data) {
			NodeListTem* temp = new (NodeListTem);
			temp->prev = nullptr;
			temp->data = _data;
			if (!Head) {
				temp->next = nullptr;
				Head = Tail = temp;
			}
			else {
				temp->next = Head;
				Head->prev = temp;
				Head = temp;
			}
			_size++;
		}

		void push_back(T _data) {
			NodeListTem* temp = new NodeListTem;
			temp->next = nullptr;
			temp->data = _data;
			if (!Head) {
				temp->prev = nullptr;
				Head = Tail = temp;
			}
			else {
				temp->prev = Tail;
				Tail->next = temp;
				Tail = temp;
			}
			_size++;
		}

		void pop_front() {
			NodeListTem* del = Head;
			if (Head == nullptr)
				return;

			else if (Head == Tail) {
				Head == Tail == nullptr;
				delete del;
			}
			else {
				Head = Head->next;
				delete del;
			}
			_size--;
		}

		void pop_back() {
			NodeListTem* del = Tail;
			if (Head == Tail) {
				Head == Tail == nullptr;
				delete del;
			}
			else {
				Tail = Tail->prev;
				delete del;
			}
			_size--;
		}

		void clear() {
			NodeListTem* temp = Head;
			NodeListTem* del;
			while (temp != nullptr) {
				del = temp;
				temp = temp->next;
				delete del;
				_size--;
			}
			Head = Tail = nullptr;
		}

		T& front() { return Head->data; }
		int size() { return _size; }
		int size() const { return _size; }
		bool empty() { return _size; }

		bool operator ==(list& other) {
			NodeListTem* first = this->Head;
			NodeListTem* second = other->Head;
			while (first && second)
			{
				if (first != second) {
					return false;
				}
				else {
					first = first->next;
					second = second->next;
				}
			}
			if (!(first) && !(second))
				return true;
			else
				return false;
		}

		list& operator =(list& other) {
			NodeListTem* temp = other->Head;
			this->clear();
			while (temp) {
				this->push_back(temp->data);
				temp = temp->next;
			}
		
		}

		Iterator begin() { return Iterator(Head); }
		const_Iterator begin() const { return const_Iterator(Head); }
		Iterator end() { return Iterator(nullptr); }
		const_Iterator end() const { return const_Iterator(); }
		Iterator rbegin() { return Iterator(Tail); }
		Iterator rend() { return Iterator(nullptr); }
		const_Iterator cbegin() { return const_Iterator(Head); }
		const_Iterator cend() { return const_Iterator(nullptr); }

		Iterator Insert(Iterator itr, T _data) {
			NodeListTem* temp = new NodeListTem;;
			if (itr == end()) {
				if (Head == nullptr) {
					temp->next = nullptr;
					temp->prev = nullptr;
					temp->data = _data;
					Head = Tail = temp;
				}
			}
			else {
				NodeListTem* pre = itr.ptr->prev;
				temp->next = itr.ptr;
				temp->prev = pre;
				temp->data = _data;
				itr.ptr->prev = temp;
				if (itr.ptr == Head)
					Head = temp;
				else
					pre->next = temp;
			}
			_size++;
			return Iterator(temp);
		}

		Iterator erase(const Iterator& itr) {
			Iterator pre(itr.ptr->prev);
			pre.ptr->next = itr.ptr->next;
			itr.ptr->next->prev = pre.ptr;
			delete itr.ptr;
			if (itr.ptr == Head)
				Head = itr.ptr->next;
			if (itr.ptr == Tail)
				Tail = itr.ptr->prev;
			_size--;
			return pre;
		}
		Iterator erase(const Iterator& first, const Iterator& last) {
			Iterator temp;
			Iterator pre(first.ptr->prev);
			Iterator nex(last.ptr->next);
			pre.ptr->next = nex.ptr;
			nex.ptr->prev = pre.ptr;
			while (first != last) {
				temp(first.ptr);
				delete temp.ptr;
				++first;
				_size--;
			}
			delete last.ptr;
			return pre;
		}

	};


}
