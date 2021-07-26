#include <iostream>
using namespace std;
#include "District.h"
#include "Citizen.h"
#include "Exceptions.h"


namespace Election {

	const int Citizen::getDisrict_id() const { return district.getID(); }
	
	void Citizen::setName(const string& _name) {
		if (_name.size() == 0)
			throw invalid_argument("Invalid name.");
		name = _name;
	}
	void Citizen::setId(int _id) {
		this->id = _id;	
	}



	const bool Citizen::isVoted() {
		if (vote == -1)
			return false;
		else
			return true;
	}

	void Citizen::save(ostream& out) const {
		if (!out)
			throw SaveFailed();

		int size = name.length();
		out.write(rcastcc(&size), sizeof(size));
		out.write(rcastcc(&name[0]), size);

		out.write(rcastcc(&id), sizeof(id));
		out.write(rcastcc(&birth_year), sizeof(birth_year));
		out.write(rcastcc(&vote), sizeof(vote));
		out.write(rcastcc(&belong_to_party), sizeof(belong_to_party));
		int district_id = this->getDisrict_id();
		out.write(rcastcc(&district_id), sizeof(district_id));
		if (!out.good())
			throw SaveFailed();
	}
}