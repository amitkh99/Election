#pragma once
#ifndef DISTRICT_H
#define DISTRICT_H 1
#include "Citizen.h"
#include <vector>
#include <array>
#include "list.h"
#include "sort.h"


using namespace std;


namespace Election {
	enum { Union, Divided };
	class District{

		struct election_result {
			int total_votes;
			int arrays_size;
			vector<int> votes_per_party;
			vector<float> precent_per_party;
			vector<int> representives_per_party;
			vector<int> sorted_party_by_size;
			int winning_party_id;
		};
		protected:
			string name;
			int id;
			list<Citizen*> voters_list;
			float vote_precents;
			int representives_size;
			election_result *res;

			const int calculate_votes_precents(const int num_of_parties);
			void sortByVote(const int num_of_parties);
		public:
			
			District(const string& name, int reprensentives_size, int district_id);
			District(istream& in);
			~District();
			District(District& di) = delete;

			const string& getName() const { return name; }
			const int getID() const { return id; }
			const list<Citizen*>& getVotersList() const { return voters_list; }
			const int getVotePrecent() const { return vote_precents; }
			const int getRepresentives_size() const { return representives_size; }
			const int getWinnerParty() const { return res->winning_party_id; }
			const election_result* getRes() const { return res; }

			const int getVotesForParty(int party_id) { return res->votes_per_party[party_id]; }
			const int getRepresentiveForParty(int party_id) { return res->representives_per_party[party_id]; }

			void addCitizen(const string& name, int id,int birth_year);
			void addCitizen(Citizen* citizen);
			Citizen* searchID(const int id) const;
			void setID(const int id) { this->id = id; }
			void setVotePrecents(const int precent) { this->vote_precents = precent; }
			void clearRes() { if (res) delete res; }
			
			virtual const int getType() const = 0; /* Pure Virtual */
			virtual const string getStringType() const = 0;
			virtual void updateElectionResult(vector<int>& representives_per_party, int& max_representives, int& max_index, const int num_of_parties) = 0;
			
			const int sum_votes(const int num_of_parties);
			void save(ostream& out) const;

			friend ostream& operator<<(ostream& os,const District& dis) {
				os << "District ID: " << dis.getID() << ",District type: " << dis.getStringType() << ", Name: " << dis.getName()
					<< ", Number of representatives: " << dis.getRepresentives_size()<<endl;
				return os;
			}
	};

	class Union_District : public District {
	public:
		Union_District(const string& name, int reprensentives_size, int district_id) : District(name, reprensentives_size, district_id) {}
		Union_District(istream& in) : District(in) {}
		virtual const int getType() const override { return Union; }
		virtual const string getStringType() const override { return "Union"; }
		virtual void updateElectionResult(vector<int>& representives_per_party, int& max_representives, int& max_index, const int num_of_parties) override;
	};
	class Divided_District : public District {
	public:
		Divided_District(const string& name, int reprensentives_size, int district_id) : District(name, reprensentives_size, district_id) {}
		Divided_District(istream& in) : District(in) {}
		virtual const int getType() const override { return Divided; }
		virtual const string getStringType() const override { return "Divided"; }
		virtual void updateElectionResult(vector<int>& representives_per_party, int& max_representives, int& max_index, const int num_of_parties) override;
	};
}
#endif