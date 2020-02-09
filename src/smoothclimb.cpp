#include "board.h"


int main(int argc, char const *argv[])
{
	int size = 14;
	Board b = Board(size, 0.1);
	std::cout << b << std::endl;
	std::cout << b.getTotalSpaces() << std::endl;
	std::vector<std::tuple<int,int>> tuples = b.getLadders();
	std::cout << "Ladders: \n" << std::endl;
	for(std::tuple<int,int> tup: tuples){
		std::cout << "(" << std::get<0>(tup) << "," << std::get<1>(tup) << ")" << std::endl;
	}

}

