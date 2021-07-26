#include <iostream>
using namespace std;
#include "Election.h"
#include "Exceptions.h"
#include <algorithm>



namespace Election {
	
	ElectionDay::ElectionDay(int d, int m, int y) {
		day = d;
		month = m;
		year = y;
		num_of_parties = 0;
		size_parties = INITIAL_SIZE;
		num_of_districts = 0;
		size_districts = INITIAL_SIZE;
		election_result.president = nullptr;
		election_result.winning_party = nullptr;
		election_result.election_ended = false;
	}

	ElectionDay::~ElectionDay() {
		for (int i = 0; i < num_of_parties; i++)
			delete parties[i];

		for (int i = 0; i < num_of_districts; i++)
			delete districts[i];

		for (auto itr = voter_register.begin(); itr != voter_register.end(); itr++) {
			Citizen* temp = *itr;
			delete temp;
		}
	}
	/*Function get district name and representive size, Function create new District and insert its pointer into array in the district id place*/
	void ElectionDay::addDistrict(const string& name, int representive_size, bool district_type) {
		if (representive_size <= 0)
			throw invalid_argument("Invalid representives size.");
		District* temp_dis = nullptr;
		if (district_type == Union)
			temp_dis = new Union_District(name, representive_size, num_of_districts);
		else if (district_type == Divided)
			temp_dis = new Divided_District(name, representive_size, num_of_districts);
		else 
			throw invalid_argument("Invalid district type.");


		districts.push_back(temp_dis);
		num_of_districts++;
	}
	/*Function get data of citizen and add it into voter_register, and to district voter_register. return false if id allready exist/no match for district_id*/
	void ElectionDay::addCitizen(const string& name, int id, int birth_year, int district_id) {
		if (district_id >= num_of_districts)
			throw DistrictNotExist();

		int i,temp_id = id;
		for (i = 0; i < ID_DIGITS_NUM && temp_id != 0; i++)
			temp_id /= 10;
		
		if (i != ID_DIGITS_NUM || temp_id != 0)
			throw CitizenError("Invalid ID.");
		
		Citizen* temp = new Citizen(name, id, birth_year, districts[district_id]);
		voter_register.push_back(temp);
		districts[district_id]->addCitizen(temp);
	}
	void ElectionDay::addCitizen(Citizen* citizen) {
		voter_register.push_back(citizen);
	}
	Citizen* ElectionDay::searchID(const int id) const {
		for (auto itr = voter_register.begin(); itr != voter_register.end(); itr++) {
			Citizen* temp = *itr;
			if (temp->getId() == id)
				return temp;
		}
		return nullptr; /*Citizen doesnt exist*/
	}
	/*Function get party name and ID for head of party, Adding party into array in the Party_ID place that given by Party constructor. Return false if head of party doesnt exist*/
	void ElectionDay::addParty(const string& name, int head_id){
		Citizen* head;
		if (!(head = searchID(head_id))) 
			throw CitizenNotExist();
		
		parties.push_back(new Party(name, head, num_of_parties, num_of_districts));
		num_of_parties++;
	}
	/*Get id of candidate, party_id and district_id that the candidate running to. return false if party_id/district_id/candidate id doesnt exist. */
	void ElectionDay::addPartyCandidate(int id, int party_id, int district_id) {
		if (district_id >= num_of_districts) 
			throw DistrictNotExist();
		
		if (party_id >= num_of_parties) 
			throw PartyNotExist();
		
		Citizen* candidate = searchID(id);
		if (!candidate) 
			throw CitizenNotExist();
		if (candidate->isBelongToParty())
			throw CitizenError("Citizen allready belong to party.");
		
		parties[party_id]->addCandidateToDistrict(candidate, district_id);
	}
	/*Function sum votes for all districts, and count amount of representives per party and update the election winner - which his party got the highest representives amount*/
	void ElectionDay::sumVotes() {

		if (election_result.election_ended)
			clearRes();
		else
			election_result.election_ended = true;

		election_result.representives_per_party.resize(num_of_parties);
		for (int i = 0; i < num_of_parties; i++)
			election_result.representives_per_party[i] = 0;
		
		int max, max_ind;
		for (int i = max = max_ind = 0; i < num_of_districts; i++) {
			districts[i]->sum_votes(num_of_parties);
			districts[i]->updateElectionResult(election_result.representives_per_party, max, max_ind, num_of_parties); /*Updating representives per party according to district distribution votes*/
			try {
				for (int k = 0; k < num_of_parties; k++) {
					if (districts[i]->getRepresentiveForParty(k) > (parties[k]->getDistrictCandidatesArr(i).size())) {
						throw ResError("Not enough representives.");
					}
					parties[k]->updateElectionRes(districts[i]->getVotesForParty(k), districts[i]->getRepresentiveForParty(k));
				}
			}
			catch (exception&) {
				clearRes(); /*Remove all counted data*/
				throw;
			}
		}
		election_result.winning_party = parties[max_ind];
		election_result.president = parties[max_ind]->getHeadCand();
		sortPartiesByRepresentives();
	}
	void ElectionDay::clearRes() {
		for (int i = 0;i < districts.size();i++)
			districts[i]->clearRes();
		for (int i = 0; i < parties.size(); i++)
			parties[i]->clearRes();
		election_result.election_ended = false;
		election_result.winning_party = nullptr;
		election_result.representives_per_party.clear();
		election_result.sorted_parties_by_representives.clear();
		election_result.president = nullptr;
	}

	void ElectionDay::sortPartiesByRepresentives() {
		struct Pair {
			Party* party;
			int representives;

			bool operator<(Pair& other) { return (this->representives < other.representives); }
		};

		election_result.sorted_parties_by_representives.resize(num_of_parties);
		vector<Pair> temp;
		temp.resize(num_of_parties);

		for (int i = 0; i < num_of_parties; i++) {
			temp[i].party = parties[i];
			temp[i].representives = election_result.representives_per_party[i];
		}
		
		Qsort(temp.begin(), temp.end()); /*Sort by representives size from low to high!*/

		for (int i = 0; i < num_of_parties; i++) { /*Copy data from sorted array - from high to low*/
			election_result.sorted_parties_by_representives[i] = temp[num_of_parties - 1 - i].party;
			election_result.representives_per_party[i] = temp[num_of_parties - 1 - i].representives;
		}

	}
	/*Function get citizen id and desired party_id to vote for, if party/citizen doesnt exist or citizen allready votes return false. Else update vote for desired party.*/
	void ElectionDay::makeVote(int id, int party_id) {
		if (party_id >= num_of_parties) {
			throw PartyNotExist();
		}
		Citizen* temp = searchID(id);
		if (!temp) {
			throw CitizenNotExist();
		}
		if (this->year - temp->getBirth_year() < MINIMAL_AGE_FOR_VOTE)
			throw CitizenError("Citizen didn't reach minimal age for vote.");

		if (temp->isVoted()) {
			throw CitizenError("Citizen allready voted.");
		}
		temp->setVote(party_id,parties[party_id]);
	}
	/*Printing election resluts*/
	void ElectionDay::printResult() const {
		for (int i = 0; i < num_of_districts; i++) {
			cout << *(districts[i]);
			cout << "Turnout: " << districts[i]->getVotePrecent() << "%" << endl;

			if (districts[i]->getType() == Union) {
				cout << "Winner candidate: ";
				cout << *(const_cast<Citizen*>((parties[districts[i]->getWinnerParty()])->getHeadCand()));
			}
			else if (districts[i]->getType() == Divided) {
				cout << "Candidates that their party given representives by district: (High to low):" << endl;
				for (int k = 0; k < num_of_parties; k++) {
					int party = districts[i]->getRes()->sorted_party_by_size[k];
					if (districts[i]->getRes()->representives_per_party[party] > 0) {
						cout << *(const_cast<Citizen*>((parties[party])->getHeadCand()));
						cout << "Num of representives given to party: " << districts[i]->getRes()->representives_per_party[party] << endl;
					}
				}
			}

			cout << endl;
			cout << "District vote result:" << endl;
			for (int k = 0; k < num_of_parties; k++) {
				int party = districts[i]->getRes()->sorted_party_by_size[k];
				cout << "***Party: " << parties[party]->getName() << endl << "Total votes: " << districts[i]->getRes()->votes_per_party[party] << ", Precents: " << districts[i]->getRes()->precent_per_party[party] << endl << "Party choosen representives:" << endl << endl;
				for (int j = 0; j < districts[i]->getRes()->representives_per_party[party]; j++) {
					cout << j + 1 << ". ";
					cout << *((parties[party]->getCandidatesArr(i))[j]);
					cout << endl;
				}
			}
			cout << endl << endl;
		}
		cout << "Parties sorted by choosen representives amount: (High to low)" << endl << endl;
		for (int i = 0; i < num_of_parties; i++) {
			Party& curr_party = *(election_result.sorted_parties_by_representives[i]);
			cout << curr_party;
			cout << "Representives number: " << election_result.representives_per_party[i] << endl;
			cout << "Total votes: " << curr_party.getTotalVotes() << endl;
			cout << endl;
		}
	}

	void ElectionDay::save(ostream& out) const {
		if (!out)
			throw SaveFailed();
		const int type = this->getType();
		out.write(rcastcc(&type), sizeof(type));
		out.write(rcastcc(&day), sizeof(day));
		out.write(rcastcc(&month), sizeof(month));
		out.write(rcastcc(&year), sizeof(year));
		/*Districts save*/
		out.write(rcastcc(&num_of_districts), sizeof(num_of_districts));
		for (int i = 0; i < num_of_districts; i++) {
			districts[i]->save(out);
		}

		///*Voter Register*/
		//voter_register.save(out);
			
		/*Parties save*/
		out.write(rcastcc(&num_of_parties), sizeof(num_of_parties));
		for (int i = 0; i < num_of_parties; i++) 
			parties[i]->save(out);
					
	
		out.write(rcastcc(&election_result.election_ended), sizeof(election_result.election_ended));
		if (election_result.election_ended) { /*Saving election data if it has*/
			int id = election_result.winning_party->getID();
			out.write(rcastcc(&id), sizeof(id));
			for (int i = 0;i<num_of_parties;i++) {
				id = election_result.sorted_parties_by_representives[i]->getID();
				out.write(rcastcc(&id), sizeof(id));
			}
			for (int i = 0; i < num_of_parties; i++) {
				int num = election_result.representives_per_party[i];
				out.write(rcastcc(&num), sizeof(num));
			}
			id = election_result.president->getId();
			out.write(rcastcc(&id), sizeof(id));
		}
		if (!out.good())
			throw SaveFailed();
	}
	ElectionDay::ElectionDay(istream& in) {
		if (!in) 
			throw LoadFailed();
		
		in.read(rcastc(&day), sizeof(day));
		in.read(rcastc(&month), sizeof(month));
		in.read(rcastc(&year), sizeof(year));

		if (in.eof()) 
			throw LoadFailed();

		

		/*Districts Load*/
		in.read(rcastc(&num_of_districts), sizeof(num_of_districts));
		districts.resize(num_of_districts);

		for (int i = 0; i < num_of_districts; i++) {
			int type;
			in.read(rcastc(&type), sizeof(type));
			if (type == Union)
				districts[i] = new Union_District(in);
			if (type == Divided)
				districts[i] = new Divided_District(in);
			for (auto itr = districts[i]->getVotersList().begin(); itr != districts[i]->getVotersList().end(); itr++) { /*voter_register load*/
				Citizen* temp = *itr;
				addCitizen(temp);
			}
		}
		if (in.eof())
			throw LoadFailed();

		/*Load parties*/
		in.read(rcastc(&num_of_parties), sizeof(num_of_parties));
		parties.resize(num_of_parties);
		for (int i = 0; i < num_of_parties; i++) 
			parties[i] = new Party(in, voter_register);

		/*Updating vote ptr*/
		for (auto itr = voter_register.begin(); itr != voter_register.end(); itr++) { /*voter_register load*/
			Citizen* temp = *itr;
			if (temp->getVote() != -1)
				temp->setVotePtr(parties[temp->getVote()]);	
		}

		if (in.eof())
			throw LoadFailed();

		/*Loading election result if exist*/
		in.read(rcastc(&election_result.election_ended), sizeof(election_result.election_ended));

		if (election_result.election_ended) { /*Saving election data if it has*/
			int id;
			in.read(rcastc(&id), sizeof(id));
			election_result.winning_party = parties[id];

			election_result.sorted_parties_by_representives.resize(num_of_parties);
			election_result.representives_per_party.resize(num_of_parties);

			for (int i = 0; i < num_of_parties; i++) {
				in.read(rcastc(&id), sizeof(id));
				election_result.sorted_parties_by_representives[i] = parties[id];
			}
			for (int i = 0; i < num_of_parties; i++) {
				int num;
				in.read(rcastc(&num), sizeof(num));
				election_result.representives_per_party[i] = num;
			}
			in.read(rcastc(&id), sizeof(id));
			election_result.president = searchID(id);
		}
		if (!in.good())
			throw LoadFailed(); /*check if last read was good*/
	}

	ostream& operator<<(ostream& os, const list<Citizen*>& list) {
		for (auto itr = list.begin(); itr != list.end(); itr++) {
				Citizen* tempCit = *itr;
				cout << *tempCit;
			}
			return os;
	}
}