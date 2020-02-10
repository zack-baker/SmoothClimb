#include "board.h"


int main(int argc, char const *argv[])
{
	int size = 10;
	Board b = Board(size, 6, 0.05);
	std::cout << b << std::endl;
	std::cout << b.getTotalSpaces() << std::endl;
	std::vector<int*> tuples = b.getLadders();
	std::cout << "Ladders: \n" << std::endl;
	for(int* pair: tuples){
		std::cout << "(" << pair[0] << "," << pair[1] << ")" << std::endl;
	}

	std::vector<int> rolls = b.calculateOptimalRolls();
	for(int i=1;i<rolls.size();i++){
		std::cout << "Roll " << i << " is " << rolls.at(i-1) << std::endl;
	}

}

