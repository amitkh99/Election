#include "District.h"
#include "Citizen.h"
#include "Exceptions.h"
#include <iostream>


namespace	Election {

	District::District(const string& name, int reprensentives_size, int district_id) {
		if (name.length() == 0)
			throw invalid_argument("Invalid name.");
		this->name = name;
		this->representives_size = reprensentives_size;
		this->id = district_id;
		this->vote_precents = 0;
		this->res = nullptr;
	}
	District::~District() {
		if (res) {
			delete res;
		}
	}

	void District::addCitizen(const string& name, int id, int birth_year) {
		voters_list.push_back(new Citizen(name, id, birth_year, this));
	}
	void District::addCitizen(Citizen* citizen) {
		voters_list.push_back(citizen);
	}
	Citizen* District::searchID(const int id) const {
		for (auto itr = voters_list.begin(); itr != voters_list.end(); itr++) {
			Citizen* temp = *itr;
			if (temp->getId() == id)
				return temp;
		}
		return nullptr; /*Citizen doesnt exist*/
	}
	/*Function get num of parties participants in the elections. Function sums up all votes and return INDEX of the winner party in the district*/
	const int District::sum_votes(const int num_of_parties) {
		res = new election_result;
		res->total_votes = 0;
		res->arrays_size = num_of_parties;
		res->votes_per_party.resize(num_of_parties);

		for (int i = 0; i < num_of_parties; i++)
			res->votes_per_party[i] = 0;

		for (auto itr = voters_list.begin(); itr != voters_list.end(); itr++) {
			Citizen* temp = *itr;
			if (temp->isVoted()) {
				res->total_votes++;
				res->votes_per_party[temp->getVote()]++;
			}
		}
		this->vote_precents = (static_cast<float>(res->total_votes) / voters_list.size()) * 100;
		return calculate_votes_precents(num_of_parties); /*Call to function that calculate vote data, and returning index of winner*/
	}

	/*Function get number of parties and calculate how many representive choosen from each party according to vote data. Returning index of winner party*/
	const int District::calculate_votes_precents(const int num_of_parties) {
		res->precent_per_party.resize(num_of_parties);
		res->representives_per_party.resize(num_of_parties);
		int max, max_ind;
		float elector = res->total_votes / static_cast<float>(representives_size);
		float max_remind = 0;
		int max_remind_ind = 0;

		vector<float> reminds_arr;
		reminds_arr.resize(num_of_parties);

		float total_remind = 0;
		int electors_left = representives_size;

		for (int i = max = max_ind = 0; i < num_of_parties; i++) {
			res->precent_per_party[i] = (res->votes_per_party[i] / static_cast<float>(res->total_votes)) * 100;
			electors_left -= res->representives_per_party[i] = res->votes_per_party[i] / elector;
			total_remind += reminds_arr[i] = (res->votes_per_party[i] / elector) - res->representives_per_party[i];
			if (reminds_arr[i] > max_remind) {
				max_remind = reminds_arr[i];
				max_remind_ind = i;
			}
			if (res->votes_per_party[i] > max) {
				max = res->votes_per_party[i];
				max_ind = i;
			}
		}
		if (electors_left > 0) {
			res->representives_per_party[max_remind_ind]++;
			reminds_arr[max_remind_ind] = 0;
			electors_left--;
		}
		while (electors_left > 0) {
			max_remind = 0;
			for (int i = 0; i < num_of_parties; i++) {
				if (reminds_arr[i] > max_remind) {
					max_remind = reminds_arr[i];
					max_remind_ind = i;
				}				
			}
			res->representives_per_party[max_remind_ind]++;
			reminds_arr[max_remind_ind] = 0;
			electors_left--;
		}
		res->winning_party_id = max_ind;
		sortByVote(num_of_parties);
		return max_ind;
	}

	void District::sortByVote(const int num_of_parties) {
	struct Pair {
		int votes;
		int party_id;
		bool operator<(Pair& other) { return (this->votes < other.votes); }
	};
	vector<Pair> temp;
	temp.resize(num_of_parties);
	res->sorted_party_by_size.resize(num_of_parties);
	for (int i = 0; i < num_of_parties; i++) {
		temp[i].votes = res->votes_per_party[i];
		temp[i].party_id = i;
	}
	Qsort(temp.begin(), temp.end());

	for (int i = 0; i < num_of_parties; i++) 
		res->sorted_party_by_size[i] = temp[num_of_parties - 1 - i].party_id; /*The sort function sorted the array from low to high*/
	
}

	void Union_District::updateElectionResult(vector<int>& representives_per_party, int& max_representives, int& max_index, const int num_of_parties) {
		representives_per_party[getWinnerParty()] += getRepresentives_size();
		if (representives_per_party[getWinnerParty()] > max_representives) {
			max_representives = representives_per_party[getWinnerParty()];
			max_index = getWinnerParty();
		}
	}
	void Divided_District::updateElectionResult(vector<int>& representives_per_party, int& max_representives, int& max_index, const int num_of_parties) {
		for (int j = 0; j < num_of_parties; j++) {
			representives_per_party[j] += getRepresentiveForParty(j);
			if (representives_per_party[j] > max_representives) {
				max_representives = representives_per_party[j];
				max_index = j;
			}
		}
	}

	void District::save(ostream& out) const {
		if (!out)
			throw SaveFailed();
		int type = this->getType();
		out.write(rcastcc(&type), sizeof(type));
		int size = name.length();
		out.write(rcastcc(&size), sizeof(size));
		out.write(rcastcc(&name[0]), size);
		out.write(rcastcc(&id), sizeof(id));

		/*Saving voters list*/
		size = voters_list.size();
		out.write(rcastcc(&size), sizeof(size));
		for (auto itr = voters_list.begin(); itr != voters_list.end(); itr++) {
			Citizen* temp = *itr;
			temp->save(out);
		}

		out.write(rcastcc(&vote_precents), sizeof(vote_precents));
		out.write(rcastcc(&representives_size), sizeof(representives_size));
		bool res_status; /*Saving flag*/
		if (!res) {
			res_status = false;
			out.write(rcastcc(&res_status), sizeof(res_status));
		}
		else {
			res_status = true;
			out.write(rcastcc(&res_status), sizeof(res_status));
			out.write(rcastcc(&(res->total_votes)), sizeof(res->total_votes));
			out.write(rcastcc(&(res->arrays_size)), sizeof(res->arrays_size));
			for (int i = 0;i<res->arrays_size;i++)
				out.write(rcastcc(&(res->votes_per_party[i])),sizeof(res->votes_per_party[i]));
			for (int i = 0; i < res->arrays_size; i++)
				out.write(rcastcc(&(res->precent_per_party[i])), sizeof(res->precent_per_party[i]));
			for (int i = 0; i < res->arrays_size; i++)
				out.write(rcastcc(&(res->representives_per_party[i])), sizeof(res->representives_per_party[i]));
			for (int i = 0; i < res->arrays_size; i++)
				out.write(rcastcc(&(res->sorted_party_by_size[i])), sizeof(res->sorted_party_by_size[i]));
			out.write(rcastcc(&(res->winning_party_id)), sizeof(res->winning_party_id));
		}
		if (!out.good())
			throw SaveFailed();
	}
	District::District(istream& in) { /*District_type readed from calling function*/

		int name_size;
		in.read(rcastc(&name_size), sizeof(name_size));
		name.resize(name_size);
		in.read(rcastc(&name[0]), name_size);

		in.read(rcastc(&id), sizeof(id));

		/*Voters register load*/
		int voters_size;
		in.read(rcastc(&voters_size), sizeof(voters_size));
		for (int i = 0; i < voters_size; i++) {
			int name_size, id, birth_year, vote, district_id;
			bool belong_to_party;
			in.read(rcastc(&name_size), sizeof(name_size));
			string name;
			name.resize(name_size);
			in.read(rcastc(&name[0]), name_size);

			in.read(rcastc(&id), sizeof(id));
			in.read(rcastc(&birth_year), sizeof(birth_year));
			in.read(rcastc(&vote), sizeof(vote));
			in.read(rcastc(&belong_to_party), sizeof(belong_to_party));
			in.read(rcastc(&district_id), sizeof(district_id));
			
			Citizen* loaded_citizen = new Citizen(name, id, birth_year, this, vote, belong_to_party);
			voters_list.push_back(loaded_citizen);
		}


		in.read(rcastc(&vote_precents), sizeof(vote_precents));
		in.read(rcastc(&representives_size), sizeof(representives_size));

		bool res_status;
		in.read(rcastc(&res_status), sizeof(res_status));
		if (!res_status) 
			res = nullptr;
		else {
			res = new election_result;
			in.read(rcastc(&(res->total_votes)), sizeof(res->total_votes));
			in.read(rcastc(&(res->arrays_size)), sizeof(res->arrays_size));
			res->votes_per_party.resize(res->arrays_size);
			res->precent_per_party.resize(res->arrays_size);
			res->representives_per_party.resize(res->arrays_size);
			res->sorted_party_by_size.resize(res->arrays_size);
			for (int i = 0; i < res->arrays_size; i++) {
				int n;
				in.read(rcastc(&n), sizeof(n));
				res->votes_per_party[i] = n;
			}
			for (int i = 0; i < res->arrays_size; i++) {
				float n;
				in.read(rcastc(&n), sizeof(n));
				res->precent_per_party[i] = n;
			}
			for (int i = 0; i < res->arrays_size; i++) {
				int n;
				in.read(rcastc(&n), sizeof(n));
				res->representives_per_party[i] = n;
			}
			for (int i = 0; i < res->arrays_size; i++) {
				int n;
				in.read(rcastc(&n), sizeof(n));
				res->sorted_party_by_size[i] = n;
			}
			in.read(rcastc(&(res->winning_party_id)), sizeof(res->winning_party_id));
		}

	
	}


}