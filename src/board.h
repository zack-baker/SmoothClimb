#include <vector>
#include <iostream>
#include <random>

class Board
{
public:
	Board(int, int, double);
	~Board();
	//getters
	int getSize();
	std::vector<int*> getLadders();
	//setters
	void setSize(int);


	std::vector<int> calculateOptimalRolls();
	int getTotalSpaces();


	friend std::ostream& operator<<(std::ostream&, const Board&);

private:
	int size;
	int dicesize;
	std::vector<int*> ladders;	

};