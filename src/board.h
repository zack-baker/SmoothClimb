#include <vector>
#include <iostream>
#include <tuple>
#include <random>

class Board
{
public:
	Board(int, double);
	~Board();
	//getters
	int getSize();
	std::vector<std::tuple<int,int>> getLadders();
	//setters
	void setSize(int);


	std::vector<int> calculateOptimalRolls();
	int getTotalSpaces();
	friend std::ostream& operator<<(std::ostream&, const Board&);

private:
	int size;
	std::vector<std::tuple<int,int>> ladders;	

};