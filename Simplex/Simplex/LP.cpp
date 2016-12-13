#include "LP.h"
#include <fstream>

void LP::read_file(std::string filename)
{
	std::ifstream fin(filename);
	fin >> m >> n;
	double tmp;
	for (std::size_t i = 0; i < n; ++i)
	{
		fin >> tmp;
		c.push_back(tmp);
	}
	for (std::size_t i = 0; i < m; ++i)
	{
		fin >> tmp;
		b.push_back(tmp);
	}
	for (std::size_t i = 0; i < m; ++i)
	{
		A.push_back(std::vector<double>());

		for (std::size_t j = 0; j < n; ++j)
		{
			fin >> tmp;
			A.back().push_back(tmp);
		}
	}
}

void LP::deslack()
{
	for (size_t row = 0;row<m;++row)
	{
		for (size_t i = 0; i < m; ++i)
		{
			A[row].push_back(i == row ? 1. : 0.);
		}
	}
}

LP::LP()
{
	return;
}