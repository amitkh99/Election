#pragma once
#include <algorithm>
using namespace std;
namespace Election {

	template <class T>
	T Partion(T begin, T end) {
		T ind = --end;
		T pivot = begin;
		bool ind_right_then_pivot = true;

		while (pivot != ind) {
			if (ind_right_then_pivot) {
				if (*ind < *pivot) {
					swap(*pivot, *ind);
					swap(pivot, ind);
					ind_right_then_pivot = false;
					ind++;
				}
				else {
					ind--;
				}
			}
			else if (!ind_right_then_pivot) {
				if (*pivot < *ind) {
					swap(*pivot, *ind);
					swap(pivot, ind);
					ind_right_then_pivot = true;
					ind--;
				}
				else {
					ind++;
				}
			}
		}
		return pivot;
	}
	template <class T>
	void Qsort(T begin, T end) {
		if (begin != end) {
			T pivot = Partion(begin, end);
			Qsort(begin, pivot);
			Qsort(++pivot, end);
		}
		else
			return;
	}

	template <class T, class Comp>
	T Partion(T begin, T end, Comp cmp) {
		T ind = --end;
		T pivot = begin;
		bool ind_right_then_pivot = true;

		while (pivot != ind) {
			if (ind_right_then_pivot) {
				if (cmp(*ind, *pivot)) {
					swap(*pivot, *ind);
					swap(pivot, ind);
					ind_right_then_pivot = false;
					ind++;
				}
				else {
					ind--;
				}
			}
			else if (!ind_right_then_pivot) {
				if (cmp(*pivot, *ind)) {
					swap(*pivot, *ind);
					swap(pivot, ind);
					ind_right_then_pivot = true;
					ind--;
				}
				else {
					ind++;
				}
			}
		}
		return pivot;
	}
	template <class T, class Comp>
	void Qsort(T begin, T end, Comp cmp) {
		if (begin != end) {
			T pivot = Partion(begin, end, cmp);
			Qsort(begin, pivot, cmp);
			Qsort(++pivot, end, cmp);
		}
		else
			return;
		
	};

};