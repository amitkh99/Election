#pragma once
#include "Party.h"
#include "District.h"
#include "list.h"
#include "sort.h"
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

namespace Election {
	enum { Regular, Simple };
	class ElectionDay
	{
		struct Res {
			bool election_ended;
			Party* winning_party;
			vector<Party*> sorted_parties_by_representives;
			vector<int> representives_per_party;
			const Citizen* president;
		};
		const int INITIAL_SIZE = 2;
		const int MINIMAL_AGE_FOR_VOTE = 18;
		const int ID_DIGITS_NUM = 9;
	protected:
		int day, month, year;
		int num_of_districts;
		int size_districts;
		vector<District*> districts;
		list<Citizen*> voter_register;
		int num_of_parties;
		int size_parties;
		vector<Party*> parties;
		Res election_result;
	public:
		ElectionDay(istream& in);
		ElectionDay(int d, int m, int y);
		virtual ~ElectionDay();
		ElectionDay(ElectionDay& el) = delete;

		void addDistrict(const string& name, int representive_size, bool district_type);
		virtual void addCitizen(const string& name, int id, int birth_year, int district_id);
		virtual void addCitizen(Citizen* citizen);
		void addParty(const string& name, int head_id);
		virtual void addPartyCandidate(int id, int party_id, int district_id);
		void makeVote(int id, int party_id);
		void sumVotes();
		const bool isEnded() const { return election_result.election_ended; }
		Citizen* searchID(const int id) const;
		const int getNumDistricts() const { return num_of_districts; }
		District& getDistrictsFromArr(int index) { return *districts[index]; }
		list<Citizen*>& getVoteRegister() { return voter_register; }
		const int getNumParties() const { return num_of_parties; }
		Party& getpartiesFromArr(int index) { return *parties[index]; }
		int getNumCand(int index) { return (parties[index]->getNumCandidateseDistrict(index)); }
		vector<Citizen*>& getCitArr(int index){return (parties[index]->getCandidatesArr(index));}
		void printResult() const;
		void clearRes();
		void sortPartiesByRepresentives();

		virtual const int getType() const { return Regular; }
		void save(ostream& out) const;
		
	};
	class SimpleElection : public ElectionDay {
	public:
		SimpleElection(int d, int m, int y, int representives_size) : ElectionDay(d, m, y) { num_of_districts = 1; string dis_name = "Dis0"; districts.push_back(new Divided_District(dis_name, representives_size, 0)); }
		SimpleElection(istream& in) : ElectionDay(in) {}
		virtual void addCitizen(const string& name, int id, int birth_year, int district_id) override { ElectionDay::addCitizen(name, id, birth_year, 0); };

		virtual void addPartyCandidate(int id, int party_id, int district_id) override {  ElectionDay::addPartyCandidate(id, party_id, 0); }
		virtual const int getType() const override { return Simple; }
	};

	ostream& operator<<(ostream& os, const list<Citizen*>& list);
}