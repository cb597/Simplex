#include <vector>
#include "LP.h"
#include <iostream>

enum Status
{
	UNSOLVED,
	SOLVED,
	UNBOUNDED
};


class Tableau {
public:
	Status simplex_iteration();
	Tableau(const LP&);
	friend std::ostream& operator<<(std::ostream& os, Tableau t);
private:
	std::vector<size_t> basis;
	std::vector<double> p;
	std::vector<std::vector<double> > Q;
	double z0;
	std::vector<double> r;
	double q(size_t i, size_t alpha);
	size_t get_candidate_for_basis(); //no pivot yet
	size_t choose_basis_row_to_replace(size_t alpha);
	void replace_in_basis(size_t old_val, size_t new_val);
};