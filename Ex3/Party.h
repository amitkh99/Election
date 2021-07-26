#pragma once

#include "Citizen.h"
#include <vector>
#include "list.h"

using namespace std;

namespace Election {
	class Party {
		struct Res {
			int representives_number;
			int total_votes;
		};
	private:
	string name;
	int id;
	Citizen* head_candidate;

	int num_of_districts;
	vector<vector<Citizen*>> districts_candidates_arr;

	Res election_result;

	public:
		Party(const string& _name, Citizen* head_candidate, int party_id, int num_of_districts);
		Party(istream& in, list<Citizen*>& vote_register);
		Party(Party& pa) = delete;
		const string& getName() const { return name; }
		const int getID() const { return id; }
		const Citizen* getHeadCand() const { return head_candidate; }

		Citizen* searchID(list<Citizen*>& voter_register,const int id) const;
		void setName(const string& _name);
		void setHeadCandidate(Citizen* head) { head_candidate = head; }
		void addCandidateToDistrict(Citizen* candidate, int district_id);
		void clearRes() { election_result.representives_number = election_result.total_votes = 0; }

		void updateElectionRes(int votes, int representives) { election_result.total_votes += votes; election_result.representives_number += representives; }
		 vector<Citizen*>& getDistrictCandidatesArr(int district_id);

		const int getTotalVotes() { return election_result.total_votes; }
		const int getTotalRepresentives() { return election_result.representives_number; }
		 vector<Citizen*>& getCandidatesArr(int index) { return districts_candidates_arr[index]; }
		const int getNumCandidateseDistrict(int index) { return districts_candidates_arr.at(index).size(); }

		void save(ostream& out) const;

		friend ostream& operator<<(ostream& os,const Party& par) {
		os << "Party ID:" << par.getID() << " Name party: " << par.getName()
		<< " Candidate for head of state: " << par.head_candidate->getName()<<endl; 

		return os;
		}
	};


}