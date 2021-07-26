#pragma once
#ifndef CITIZEN_LINKED_LIST_H
#define CITIZEN_LINKED_LIST_H 1
#include <iostream>
#include <vector>
using namespace std;

namespace Election {
	class Citizen;
	class District;
	class CitizenLinkedList {
	public:
		class ListNode {
		public:
			ListNode* next;
			ListNode* prev;
			Citizen* data;
			ListNode() { next = prev = nullptr; data = nullptr; }
			ListNode(Citizen* data) { ListNode(); this->data = data; }
		};
		ListNode* head;
		ListNode* tail;
		int size;

	

		CitizenLinkedList() { head = tail = nullptr; size = 0; }
		~CitizenLinkedList();
		CitizenLinkedList(CitizenLinkedList& cit) = delete;

		ListNode* getHead() const { return head; }
		ListNode* getTail() const { return tail; } 
		int getSize() { return size; }

		void setHead(ListNode* head) { this->head = head; }
		void setTail(ListNode* tail) { this->tail = tail; }
		bool addAfter(ListNode* prev, ListNode* node);
		bool addToList(ListNode* node);
		bool addToList(Citizen* citizen);
		bool deleteNode(ListNode* node);
		bool deleteNode(Citizen* citizen);

		void pop_back() const;
		bool isEmpty();

		Citizen* searchID(int id) const;
		ListNode* searchNodeByID(int id) const;

		Citizen* addCitizen(const string& _name, int _id, int _birth_year, District* district, int _vote = -1);

		bool save(ostream& out) const;
		bool load(istream& in, vector<District*>& districts, const int num_of_districts);
		bool load(istream& in, District* district) {
			vector<District*> temp;
			temp.push_back(district);
			return load(in, temp , 1);
		}


		friend ostream& operator<<(ostream& os, const CitizenLinkedList& citLis);
	};

}

#endif