#define _CRT_SECURE_NO_WARNINGS
#include "Election_operation.h"
#include "Citizen.h"
#include "District.h"
#include "Party.h"
#include "Election.h"
#include "Exceptions.h"


#ifndef TEST 



namespace Election {
	enum { createRound = 1, loadRound, Quit };
	enum { Add_District = 1, Add_Citizen, Add_Party, Add_Representive, Print_Districts, Print_Citizen, Print_Parties, Vote, Print_Res, Exit, Save, Load };
	enum { Start, Main };

	unsigned int Election_Operation::printOptions(int menu) {
		while (true) {
			unsigned int input;
			if (menu == Start) {
				cout << "Please choose an operation: " << endl;
				cout << "1. Create a new election round" << endl;
				cout << "2. Load an existing election round" << endl;
				cout << "3. Exit" << endl;
				cin >> input;
				if (input >= 1 && input <= 3)
					return input;
				else
					cout << "Invalid selection. Try again." << endl;
			}

			if (menu == Main) {
				cout << "Please choose an operation: " << endl;
				cout << "1. Add a district" << endl;
				cout << "2. Add a citizen" << endl;
				cout << "3. Add a party" << endl;
				cout << "4. Add a citizen as a party representative" << endl;
				cout << "5. Present all the districts" << endl;
				cout << "6. Present all the citizens" << endl;
				cout << "7. Present all the parties" << endl;
				cout << "8. Voting" << endl;
				cout << "9. Present election results" << endl;
				cout << "10. Exit" << endl;
				cout << "11. Save current election round" << endl;
				cout << "12. Load exist election round" << endl;
				cin >> input;
				if (input >= 1 && input <= 12) {
						return input;
				}
				else {
					system("cls");
					cout << "** Invalid selection. Try again. **" << endl << endl;
				}
			}

		}
	}

	void Election_Operation::StartMenu() {
		while (true) {
			unsigned int input = printOptions(Start);
			try {
				if (input == createRound) {
					int d, m, y, type, size;
					cout << "Please enter election date (d/m/y) and election type (0 - Regular, 1- Simple)" << endl;
					cin >> d >> m >> y >> type;
					validDate(d, m, y);
					if (type == Regular)
						elec = new ElectionDay(d, m, y);
					else if (type == Simple) {
						cout << "Please enter representives size:" << endl;
						cin >> size;
						elec = new SimpleElection(d, m, y, size);
					}
					MainMenu();
				}
				else if (input == loadRound) {
					char file_name[100];
					cout << "Please enter file name:" << endl;
					cin >> file_name;
					LoadFromFile(file_name);
					MainMenu();
				}
				else if (input == Quit) {
					system("cls");
					cout << "Goodbye..." << endl;
					exit(0);
				}
			}
			catch (LoadFailed& exc) {
				cout << "**ERROR** --> " << exc.what();
				exit(-1);
			}
			catch (exception& exc) {
				cout << "**ERROR** --> " << exc.what() << endl << endl;
			}
		}
	}
	void Election_Operation::MainMenu() {
		while (true) {
			unsigned int input = printOptions(Main); /*Get user input*/
			system("cls");
			try {
				if (input == Add_District) {
					if (elec->getType() == Simple)
						throw invalid_argument("Invalid district type.");
					else {
						char name[100];
						int size, type;
						cout << "Please enter: Name, representives size, Type - (0 -> Union district / 1 -> Divided district)" << endl;
						cin >> name >> size >> type;
						elec->addDistrict(name, size, type);
					}
				}
				else if (input == Add_Citizen) {
					char name[100];
					int id, birth_year, district_number;
					cout << "Please enter: Name, ID, Birth year, District number" << endl;
					cin >> name >> id >> birth_year >> district_number;
					elec->addCitizen(name, id, birth_year, district_number);
				}
				else if (input == Add_Party) {
					char name[100];
					int id;
					cout << "Please enter: Party name, The ID of the candidate for the state authority" << endl;
					cin >> name >> id;
					elec->addParty(name, id);
				}
				else if (input == Add_Representive) {
					int id_rep, party_number, district_number;
					cout << "Please enter: representative ID number, party number, district number" << endl;
					cin >> id_rep >> party_number >> district_number;
					elec->addPartyCandidate(id_rep, party_number, district_number);
				}
				else if (input == Print_Districts) {
					for (int i = 0; i < elec->getNumDistricts(); i++)
						cout << elec->getDistrictsFromArr(i);
				}
				else if (input == Print_Citizen) {
					cout << elec->getVoteRegister();
				}
				else if (input == Print_Parties) {
					int num_of_candidates;
					Citizen** candidates_arr;
					for (int i = 0; i < elec->getNumParties(); i++) {
						Party& curr_party = elec->getpartiesFromArr(i);
						cout << curr_party;
						for (int k = 0; k < elec->getNumDistricts(); k++) {
							num_of_candidates = curr_party.getNumCandidateseDistrict(k);
							for (int j = 0; j < num_of_candidates; j++) {
								const Citizen& temp_cit = *(curr_party.getDistrictCandidatesArr(k))[j];
								cout << temp_cit;
							}
						}
						cout << endl;
					}
				}
				else if (input == Vote) {
					int id, party_number;
					cout << "Please enter: citizen ID number, desired party id:" << endl;
					cin >> id >> party_number;
					elec->makeVote(id, party_number);
				}
				else if (input == Print_Res) {
					elec->sumVotes();
					elec->printResult();
				}
				else if (input == Exit) {
					break;
				}
				else if (input == Save) {
					char file_name[100];
					cout << "Please enter file name to save current election round:" << endl;
					cin >> file_name;
					SaveCurrent(file_name);
				}
				else if (input == Load) {
					delete elec;
					elec = nullptr;
					char file_name[100];
					cout << "Please enter file name:" << endl;
					cin >> file_name;
					LoadFromFile(file_name);
					MainMenu();
				}
			}
			catch (bad_alloc& exc) {
				cout << "**ERROR** -->" << exc.what() << endl;
				exit(-1);
			}
			catch (LoadFailed& exc) {
				cout << "**ERROR** --> " << exc.what() << endl;
				exit(-1);
			}
			catch (exception& exc) {
				cout << "**ERROR** --> " << exc.what() << endl << endl;
			}
		}
		delete elec;
		elec = nullptr;
	}

	void Election_Operation::SaveCurrent(char* file_name) {
		ofstream out(file_name, ios::binary);
		if (!out)
			throw SaveFailed();

		elec->save(out);
		out.close();
	}

	void Election_Operation::LoadFromFile(char* file_name) {
		ifstream in(file_name, ios::binary);
		if (!in)
			throw invalid_argument("File not exist");

			if (elec)
				delete elec;
			int type;
			in.read(rcastc(&type), sizeof(type));
			try {
			if (type == Regular)
				elec = new ElectionDay(in);
			if (type == Simple)
				elec = new SimpleElection(in);
			}
			catch (...) {
				throw LoadFailed();
			}

			in.close();
	}
	enum { jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };
	void Election_Operation::validDate(int d, int m, int y) {
		if (m < jan || m > dec)
			throw invalid_argument("Invalid Date.");
		if (d >= 1 && d <= 31) {
			if (m == feb && d > 28)
				throw invalid_argument("Invalid Date.");
			if ((m == apr || m == jun || m == sep || m == nov) && d == 31)
				throw invalid_argument("Invalid Date.");
			if (y < CURRENT_YEAR)
				throw invalid_argument("Invalid Date.");
		}
		else
			throw invalid_argument("Invalid Date.");

	}
}




#else
#include <fstream>

namespace Election {
	enum { createRound = 1, loadRound, Quit };
	enum { Add_District = 1, Add_Citizen, Add_Party, Add_Representive, Print_Districts, Print_Citizen, Print_Parties, Vote, Print_Res, Exit, Save, Load };
	enum { Start, Main };

	unsigned int Election_Operation::printOptions(int menu) {
		while (true) {
			unsigned int input;
			if (menu == Start) {
				cout << "Please choose an operation: " << endl;
				cout << "1. Create a new election round" << endl;
				cout << "2. Load an existing election round" << endl;
				cout << "3. Exit" << endl;
				cin >> input;
				if (input >= 1 && input <= 3)
					return input;
				else
					cout << "Invalid selection. Try again." << endl;
			}

			if (menu == Main) {
				cout << "Please choose an operation: " << endl;
				cout << "1. Add a district" << endl;
				cout << "2. Add a citizen" << endl;
				cout << "3. Add a party" << endl;
				cout << "4. Add a citizen as a party representative" << endl;
				cout << "5. Present all the districts" << endl;
				cout << "6. Present all the citizens" << endl;
				cout << "7. Present all the parties" << endl;
				cout << "8. Voting" << endl;
				cout << "9. Present election results" << endl;
				cout << "10. Exit" << endl;
				cout << "11. Save current election round" << endl;
				cout << "12. Load exist election round" << endl;
				cin >> input;
				if (input >= 1 && input <= 12) {
					return input;
				}
				else {
					system("cls");
					cout << "** Invalid selection. Try again. **" << endl << endl;
				}
			}

		}
	}

	void Election_Operation::StartMenu() {

		while (true) {
			unsigned int input = printOptions(Start);

			if (input == createRound) {
				int d, m, y, type, size;
				cout << "Please enter election date (d/m/y) and election type (0 - Regular, 1- Simple)" << endl;
				cin >> d >> m >> y >> type;
				if (type == Regular)
					elec = new ElectionDay(d, m, y);
				else if (type == Simple) {
					cout << "Please enter representives size:" << endl;
					cin >> size;
					elec = new SimpleElection(d, m, y, size);
				}
				MainMenu();
			}
			else if (input == loadRound) {
				char file_name[100];
				cout << "Please enter file name:" << endl;
				cin >> file_name;
				LoadFromFile(file_name);
				MainMenu();
			}

			else if (input == Quit) {
				system("cls");
				cout << "Goodbye..." << endl;
				exit(0);
			}
		}
	}
	void Election_Operation::MainMenu() {
		ifstream in("test.txt");
		while (true) {
			unsigned int input;
			if (in.eof())
				input = printOptions(Main); /*Get user input*/
			in >> input;

			system("cls");
			try {
				if (input == Add_District) {
					if (elec->getType() == Simple)
						throw invalid_argument("Invalid district type.");
					else {
						char name[100];
						int size, type;
						cout << "Please enter: Name, representives size, Type - (0 -> Union district / 1 -> Divided district)" << endl;
						in >> name >> size >> type;
						elec->addDistrict(name, size, type);
					}
				}
				else if (input == Add_Citizen) {
					char name[100];
					int id, birth_year, district_number;
					cout << "Please enter: Name, ID, Birth year, District number" << endl;
					in >> name >> id >> birth_year >> district_number;
					elec->addCitizen(name, id, birth_year, district_number);
				}
				else if (input == Add_Party) {
					char name[100];
					int id;
					cout << "Please enter: Party name, The ID of the candidate for the state authority" << endl;
					in >> name >> id;
					elec->addParty(name, id);
				}
				else if (input == Add_Representive) {
					int id_rep, party_number, district_number;
					cout << "Please enter: representative ID number, party number, district number" << endl;
					in >> id_rep >> party_number >> district_number;
					elec->addPartyCandidate(id_rep, party_number, district_number);
				}
				else if (input == Print_Districts) {
					for (int i = 0; i < elec->getNumDistricts(); i++)
						cout << elec->getDistrictsFromArr(i);
				}
				else if (input == Print_Citizen) {
					cout << elec->getVoteRegister();
				}
				else if (input == Print_Parties) {
					int num_of_candidates;
					Citizen** candidates_arr;
					for (int i = 0; i < elec->getNumParties(); i++) {
						Party& curr_party = elec->getpartiesFromArr(i);
						cout << curr_party;
						for (int k = 0; k < elec->getNumDistricts(); k++) {
							num_of_candidates = curr_party.getNumCandidateseDistrict(k);
							/*					candidates_arr = curr_party.getDistrictCandidatesArr(k);*/
							for (int j = 0; j < num_of_candidates; j++) {
								const Citizen& temp_cit = *(curr_party.getDistrictCandidatesArr(k))[j];
								cout << temp_cit;
							}
						}
						cout << endl;
					}
				}
				else if (input == Vote) {
					int id, party_number;
					cout << "Please enter: citizen ID number, desired party id:" << endl;
					in >> id >> party_number;
					elec->makeVote(id, party_number);
				}
				else if (input == Print_Res) {
					elec->sumVotes();
					elec->printResult();
				}
				else if (input == Exit) {
					break;
				}
				else if (input == Save) {
					char file_name[100];
					cout << "Please enter file name to save current election round:" << endl;
					cin >> file_name;
					SaveCurrent(file_name);
				}
				else if (input == Load) {
					delete elec;
					elec = nullptr;
					char file_name[100];
					cout << "Please enter file name:" << endl;
					cin >> file_name;
					LoadFromFile(file_name);
					MainMenu();
				}
			}
			catch (LoadFailed& exc) {
				cout << "**ERROR** --> " << exc.what();
				exit(-1);
			}
			catch (exception& exc) {
				cout << "**ERROR** --> " << exc.what() << endl << endl;
			}
		}
		delete elec;
		elec = nullptr;
	}

	void Election_Operation::SaveCurrent(char* file_name) {
		ofstream out(file_name, ios::binary);
		if (!out)
			throw SaveFailed();

		elec->save(out);
		out.close();
	}

	void Election_Operation::LoadFromFile(char* file_name) {
		ifstream in(file_name, ios::binary);
		if (!in) 
			throw invalid_argument("File not exist");

			if (elec)
				delete elec;
			int type;
			in.read(rcastc(&type), sizeof(type));
			if (type == Regular)
				elec = new ElectionDay(in);
			if (type == Simple)
				elec = new SimpleElection(in);

			in.close();
	}
}


#endif 