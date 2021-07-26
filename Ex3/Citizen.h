#pragma once
#include <iostream>
#include <string.h>

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

using namespace std;
namespace Election {
	class Party;
	class District;
	class Citizen
	{
	private:
		string name;
		int id; 
		int birth_year;
		int vote;
		Party* party_voted;
		bool belong_to_party;
		District& district;

	public:
		Citizen(const string& _name, int _id, int _birth_year, District* district, int _vote = -1, bool belong_to_party = false) : id(_id), birth_year(_birth_year), district(*district), vote(_vote), belong_to_party(belong_to_party), party_voted(nullptr) { setName(_name); }
		Citizen(Citizen& ci) = delete;
		void setName(const string& name);
		void setId(int id);
		void setVote(int _vote, Party* party) {
			vote = _vote; party_voted = party;
		}
		void setVotePtr(Party* party) { party_voted = party; }
		void setBelongToParty(bool belong = true) { belong_to_party = belong; }

		const string& getName() const { return name; }
		const int getId() const { return id; }
		const int getBirth_year() const { return birth_year; }
		const District& getDisrict() const { return district; }
		const int getDisrict_id() const;
		const int getVote() const { return vote; }
		const bool isVoted();
		const bool isBelongToParty() const { return belong_to_party; }

		void save(ostream& out) const;
		
		friend ostream& operator<<(ostream& os,const  Citizen& cit) {
			os << "Name: " << cit.getName() << ", ID: " << cit.getId()
				<< ", Birth year: " << cit.getBirth_year() << ", District ID: "<<cit.getDisrict_id()<<endl;
			return os;
		}
	};

	

}