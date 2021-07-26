#include "CitizenLinkedList.h"
#include "Citizen.h"
#include "Exceptions.h"

namespace Election {

	CitizenLinkedList::~CitizenLinkedList() {
		ListNode* curr, * prev;
		curr = head;
		while (curr) {
			prev = curr;
			curr = curr->next;
			delete prev;
		}
	}

	bool CitizenLinkedList::addAfter(ListNode* prev, ListNode* node) {
		if (!prev)
			return false; //Wrong node address

		ListNode* temp = prev->next;
		prev->next = node;
		if (temp)
			temp->prev = node;
		else //New node is tail
			this->setTail(node);

		node->next = temp;
		node->prev = prev;
		size++;
		return true;
	}

	bool CitizenLinkedList::addToList(ListNode* node) {
		if (this->isEmpty()) {
			this->head = this->tail = node;
			size++;
			return true;
		}
		else 
			return this->addAfter(this->tail, node); //Insert to tail
	}

	bool CitizenLinkedList::addToList(Citizen* citizen) {
		CitizenLinkedList::ListNode* node = new CitizenLinkedList::ListNode(citizen);
		if (addToList(node))
			return true;
		else {
			delete node;
			return false;
		}
	}

	bool CitizenLinkedList::deleteNode(ListNode* node) {
		if (head == node) {
			head = node->next;
			if (head)
				head->prev = nullptr;
		}
		else if (tail == node) {
			tail = node->prev;
			tail->next = nullptr;
		}
		else {
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
		size--;
		delete node;
		return true;
	}
	bool CitizenLinkedList::deleteNode(Citizen* citizen) {
		ListNode* curr = searchNodeByID(citizen->getId());
		if (!curr)
			return false;
		else
			return deleteNode(curr);
	}

	bool CitizenLinkedList::isEmpty() {
		if (head)
			return false;
		else
			return true;
	}
	/*Get citizen id number, return false if id doesnt exist. else return pointer to desired citizen*/
	Citizen* CitizenLinkedList::searchID(int id) const {
		ListNode* curr = searchNodeByID(id);
		if (!curr)
			return nullptr;
		else
			return curr->data;
	}
	/*Get citizen id number, return false if id doesnt exist. else return pointer to desired citizen NODE of the linked list*/
	CitizenLinkedList::ListNode* CitizenLinkedList::searchNodeByID(int id) const {
		ListNode* curr = this->head;
		while (curr) {
			if (id == curr->data->getId())
				return curr;
			curr = curr->next;
		}
		return nullptr; //Citizen doesn't excist
	}
	/*Get citizen data, if citizen id exist allready return false. else the function create new ListNode with citizen data and add it into the list, return true */
	Citizen* CitizenLinkedList::addCitizen(const string& _name, int _id, int _birth_year, District* district, int _vote) {
		if ((searchID(_id))) 
			throw CitizenError("Citizen with this id allready exist");
		else {
			Citizen* NewCitizen = new Citizen(_name, _id, _birth_year, district, _vote);
			ListNode* NewNode = new ListNode(NewCitizen);
			addToList(NewNode);
		    return NewCitizen;
		}
	}
	ostream& operator<<(ostream& os, const CitizenLinkedList& citLis) {
		CitizenLinkedList::ListNode* tempNode = citLis.head;
		while (tempNode)
		{
			Citizen& tempCit = *(tempNode->data);
			cout << tempCit;
			tempNode = tempNode->next;

		}
		return os;
	}
	bool CitizenLinkedList::save(ostream& out) const {
		if (!out)
			return false;
		out.write(rcastcc(&size), sizeof(size));
		ListNode* curr = this->head;
		for (int i = 0; curr != nullptr && i < size; i++) {
			curr->data->save(out);
			curr = curr->next;
		}
		return out.good();
	}
	bool CitizenLinkedList::load(istream& in, vector<District*>& districts, const int num_of_districts) {
		if (!in)
			return false;

		int citizen_size;
		in.read(rcastc(&citizen_size), sizeof(citizen_size));
		size = 0;
		head = tail = nullptr;
		if (citizen_size == 0)
			return false;


		int name_size, id, birth_year, vote, district_id;
		bool belong_to_party;
		for (int i = 0; i < citizen_size; i++) {
			in.read(rcastc(&name_size), sizeof(name_size));
			string name;
			name.resize(name_size);
			in.read(rcastc(&name[0]), name_size);

			in.read(rcastc(&id), sizeof(id));
			in.read(rcastc(&birth_year), sizeof(birth_year));
			in.read(rcastc(&vote), sizeof(vote));
			in.read(rcastc(&belong_to_party), sizeof(belong_to_party));
			in.read(rcastc(&district_id), sizeof(district_id));
			int fixed_dis_id = district_id;
			if (district_id >= num_of_districts) /*Possible to happen while loading district voter list - the calling function sent an array with only one district*/
				fixed_dis_id = 0;
			Citizen* loaded_citizen = new Citizen(name, id, birth_year, districts[fixed_dis_id], vote, belong_to_party);
			addToList(loaded_citizen);
		}
	}


}