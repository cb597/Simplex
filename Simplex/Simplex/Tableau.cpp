#include "Tableau.h"
#include <limits>
#include <vector>
#include <functional>


/****************** Some useful vector operators *******************/
namespace {
	std::vector<double> operator*(const double& f, const std::vector<double>& dv)
	{
		std::vector<double> result(dv.size(),0);
		for (size_t v=0; v<dv.size();++v)
		{
			result[v]=f*dv[v];
		}
		return result;
	}
	std::vector<double> operator+(const std::vector<double>& lhs, const std::vector<double>& rhs)
	{
		std::vector<double> result;
		if (lhs.size() != rhs.size())
		{
			throw std::runtime_error("double vector + args have incompatible size");
		}
		for (size_t i = 0; i < lhs.size(); ++i)
		{
			result.push_back(lhs[i] + rhs[i]);
		}
		return result;
	}
	std::vector<double> operator-(const std::vector<double>& lhs, const std::vector<double>& rhs)
	{
		std::vector<double> result;
		if (lhs.size() != rhs.size())
		{
			throw std::runtime_error("double vector + args have incompatible size");
		}
		for (size_t i = 0; i < lhs.size(); ++i)
		{
			result.push_back(lhs[i] - rhs[i]);
		}
		return result;
	}
	std::vector<double> operator/(const std::vector<double>& lhs, const double& rhs)
	{
		std::vector<double> result;
		for (size_t i = 0; i < lhs.size(); ++i)
		{
			result.push_back(lhs[i] / rhs);
		}
		return result;
	}
}




double Tableau::q(size_t i, size_t alpha)
{
	return Q[i][alpha];
}

/// returns an index of a positive coefficient in r or r.size if none exists
size_t Tableau::get_candidate_for_basis()
{
	for (size_t i=0; i<r.size(); ++i)
	{
		if (r[i] > 0)
		{
			return i;
		}
	}
	return r.size();
}

/// returns row_index of basis where a critical condition is found
size_t Tableau::choose_basis_row_to_replace(size_t alpha)
{
	//find critical row
	size_t critical_idx = basis.size();
	double critical_value = std::numeric_limits<double>::lowest();
	for (size_t i = 0; i < basis.size(); ++i)
	{
		auto coef = q(i, alpha);
		if (coef >= 0 || p[i]/coef < critical_value)
		{
			continue;
		}
		// case new critical equality found
		critical_value = p[i]/coef;
		critical_idx = i;
	}
	return critical_idx;
}

void Tableau::replace_in_basis(size_t basis_row, size_t new_val)
{
	std::cout << "will replace basis variable x_" << basis[basis_row] << " by x_" << new_val << std::endl;
	
	Q[basis_row][basis[basis_row]] = -1.;
	basis[basis_row] = new_val;
	p[basis_row] /= (-Q[basis_row][new_val]);
	Q[basis_row] = Q[basis_row]/(-Q[basis_row][new_val]);
	r = r + r[new_val]*Q[basis_row];
	//p[new_val] += r[new_val] * p[basis_row];
	Q[basis_row][new_val] = 0.;

	for (size_t b = 0; b < basis.size(); ++b)
	{
		if (b == basis_row)
		{
			continue; //ignore the row we just fixed
		}

		double factor = Q[b][new_val];

		if (factor == 0)
		{
			continue; //nothing to do here
		}

		Q[b] = Q[b] + factor*Q[basis_row];
		Q[b][new_val] = 0;
		p[b] += factor*p[basis_row];

	}
}

Status Tableau::simplex_iteration()
{
	size_t candidate = get_candidate_for_basis();
	if (candidate == r.size())
	{
		return SOLVED;
	}
	size_t to_replace = choose_basis_row_to_replace(candidate);
	if (to_replace == basis.size())
	{
		return UNBOUNDED;
	}
	replace_in_basis(to_replace, candidate);
	return UNSOLVED;
}

Tableau::Tableau(const LP& lp)
{
	for (auto i : lp.b)
	{
		p.push_back(i);
	}
	for (size_t row=0; row<lp.A.size(); ++row)
	{
		Q.push_back(std::vector<double>());
		for (auto val : lp.A[row])
		{
			Q.back().push_back(-val);
		}
		for (size_t i = 0; i < lp.A.size(); ++i)
		{
			Q.back().push_back(0.);
		}
	}
	for (size_t i = lp.A.back().size(); i < lp.A.back().size() + lp.A.size(); ++i)
	{
		basis.push_back(i);
	}
	r.resize(lp.A.back().size() + lp.A.size(), 0);
	for (size_t i=0;i<lp.c.size();++i)
	{
		r[i] = lp.c[i];
	}
	
}

std::ostream & operator<<(std::ostream & os, Tableau t)
{
	for (size_t row = 0; row < t.p.size(); ++row)
	{
		os << "x_" << t.basis[row] << " = " << t.p[row];
		for (size_t col = 0; col < t.Q[0].size(); ++col)
		{
			os << " + " << t.Q[row][col] << "*x_" << col;
		}
		os << std::endl;
	}
	os << "___________________________________________________________" << std::endl;
	for (size_t col = 0; col < t.r.size(); ++col)
	{
		os << " + " << t.r[col] << "*x_" << col;
	}
	return os;
}
