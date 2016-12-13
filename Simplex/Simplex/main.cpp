#include "LP.h"
#include "Tableau.h"
#include <iostream>

int main()
{
	LP lp;
	lp.read_file("inst.txt");

	Tableau t(lp);

	Status status;

	do
	{ 
		std::cout << std::endl << t << std::endl << std::endl;
		status = t.simplex_iteration();
	}
	while (status == UNSOLVED);


	switch (status)
	{
	case UNSOLVED:
		std::cout << "How the hell did I get here?" << std::endl;
		break;
	case SOLVED:
		std::cout << "That was easy." << std::endl << t << std::endl;
		break;
	case UNBOUNDED:
		std::cout << "Stop feeding me such LPs you retard" << std::endl;
		break;
	default:
		break;
	}
}