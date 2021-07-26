#pragma once
#include <iostream>
using namespace std;

namespace Election {
	class CitizenNotExist : public exception {
	public:
		virtual const char* what() const throw() {
			return "Citizen doesn't exist.";
		}
	};
	class PartyNotExist : public exception {
	public:
		virtual const char* what() const throw() {
			return "Party doesn't exist.";
		}
	};
	class DistrictNotExist : public exception {
	public:
		virtual const char* what() const throw() {
			return "District doesn't exist.";
		}
	};
	class CitizenError : public exception {
	public:
		CitizenError(const char* what) : exception(what) {}
	};
	class ResError : public exception {
	public:
		ResError(const char* what) : exception(what) {}
	};
	class SaveFailed : public exception {
	public:
		virtual const char* what() const throw() {
			return "Save failed.";
		}
	};
	class LoadFailed : public exception {
	public:
		virtual const char* what() const throw() {
		return "Load failed.";
		}
	};
}