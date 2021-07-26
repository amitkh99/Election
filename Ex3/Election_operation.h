#pragma once
#include <iostream>
#include <fstream>
using namespace std;

namespace Election {
	class ElectionDay;
	class Election_Operation {
		const int CURRENT_YEAR = 2021;
	private:
		ElectionDay* elec;
		unsigned int printOptions(int menu);
		void StartMenu();
		void MainMenu();
		void SaveCurrent(char* file_name);
		void LoadFromFile(char* file_name);

		void validDate(int d, int m, int y);
	public:
		Election_Operation() : elec(nullptr) { StartMenu(); }
		~Election_Operation() { if (elec) { delete elec; } }
	};
}