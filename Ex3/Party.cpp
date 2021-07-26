#include <iostream>
using namespace std;
#include "Party.h"
#include "Exceptions.h"


namespace Election {
	const int INITIAL_SIZE = 2;
	Party::Party(const string& _name, Citizen* head_candidate,int party_id, int num_of_districts) {
		id = party_id;
		setName(_name);
		this->head_candidate = head_candidate;

		districts_candidates_arr.resize(num_of_districts);
		this->num_of_districts = num_of_districts;

		election_result.representives_number = 0;
		election_result.total_votes = 0;
	}

	void Party::setName(const string& _name) {
		if (_name.length() == 0)
			throw invalid_argument("Invalid name.");
		name = _name;
	}

	/*Function get pointer to citizen and id of district that the candidate belong to, Add candidate into districts candidates array.*/
	void Party::addCandidateToDistrict(Citizen* candidate, int district_id) {

		districts_candidates_arr[district_id].push_back(candidate);
		candidate->setBelongToParty();
	}
    vector<Citizen*>& Party::getDistrictCandidatesArr(int district_id) { return districts_candidates_arr.at(district_id); }
	Citizen* Party::searchID(list<Citizen*>& voter_register, const int id) const {
		for (auto itr = voter_register.begin(); itr != voter_register.end(); itr++) {
			Citizen* temp = *itr;
			if (temp->getId() == id)
				return temp;
		}
		return nullptr; /*Citizen doesnt exist*/	
	}

	void Party::save(ostream& out) const {
		if (!out)
			throw SaveFailed();
		int size = name.length();
		out.write(rcastcc(&size), sizeof(size));
		out.write(rcastcc(&name[0]), size);
		out.write(rcastcc(&id), sizeof(id));
		int temp_id = this->getHeadCand()->getId();
		out.write(rcastcc(&temp_id), sizeof(temp_id));

		out.write(rcastcc(&num_of_districts), sizeof(num_of_districts));

		size = districts_candidates_arr.size();
		out.write(rcastcc(&size), sizeof(size));

		for (int i = 0; i < size; i++) {
			const vector<Citizen*>& temp_arr = districts_candidates_arr[i];
			int temp_size = temp_arr.size();
			out.write(rcastcc(&temp_size), sizeof(temp_size));
			for (int k = 0; k < temp_size; k++) {
				const int id = (*(temp_arr[k])).getId();
				out.write(rcastcc(&id), sizeof(id));
			}

		}
		out.write(rcastcc(&election_result), sizeof(election_result));
		if (!out.good())
			throw SaveFailed();
	}
	Party::Party(istream& in, list<Citizen*>& vote_register) {
		if (!in)
			return;

		int name_size,temp_id;
		in.read(rcastc(&name_size), sizeof(name_size));
		name.resize(name_size);
		in.read(rcastc(&name[0]), name_size);

		in.read(rcastc(&id), sizeof(id));

		in.read(rcastc(&temp_id), sizeof(temp_id));
		head_candidate = searchID(vote_register,temp_id);
		in.read(rcastc(&num_of_districts), sizeof(num_of_districts));

		int arr_size;
		in.read(rcastc(&arr_size), sizeof(arr_size));
		districts_candidates_arr.resize(arr_size);
		for (int i = 0; i < arr_size; i++) {
			int temp_size;
			in.read(rcastc(&temp_size), sizeof(temp_size));
			districts_candidates_arr[i].resize(temp_size);
			for (int k = 0; k < temp_size; k++) {
				in.read(rcastc(&temp_id), sizeof(temp_id));
				districts_candidates_arr[i][k] = searchID(vote_register,temp_id);
			}
		}
		in.read(rcastc(&election_result), sizeof(election_result));
	}

}