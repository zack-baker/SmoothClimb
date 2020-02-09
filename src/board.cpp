#include "board.h"

/**
	Constructor for the board object
	@param s - the size of the board. Since the board is a 2d square, we only need one side length
	@param ladderChance - a double between 0 (inc.) and 1(exc.) used to determine if any particular space
	is a ladder source

*/
Board::Board(int s, double ladderChance){
	/** Assign the size */
	size = s;

	/** initialize ladders */
	//initialize the random number geneartion objects
	std::random_device rd;
	std::mt19937 generator(rd());
	//create the distribution object, specifying that it should generate doubles
	std::uniform_real_distribution<double> dist(0.0,1.0);
	//create a boolean array to track every space to determine if it is a ladder sink already
	bool* takenSinks = new bool[getTotalSpaces()];

	for(int space=1;space<getTotalSpaces();space++){
		//generate the random value for the current space
		double rangedRand = dist(generator);
		//if the space is chosen, add a new ladder
		if (rangedRand < ladderChance){
			//create a variable to hold the first space on the next row
			int firstNextRowSpace = space+(size-space%size+1);
			//ensure at least one space ahead of the current space is a valid sink.
			bool sinkAvail = false;
			for(int i = firstNextRowSpace;i<getTotalSpaces()+1;i++){
				if(!takenSinks[i]){
					sinkAvail = true;
					break;
				}
			}
			//if sourceAvail is false, then there are no possible sinks on the rest of the board, so stop attempting to generate ladders
			if(!sinkAvail){
				std::cout << "[DEBUG] No available sinks after space " << space << std::endl;
				for(int i=0; i< getTotalSpaces();i++){
					std::cout << "[DEBUG] sink available at space " << i << ": " << takenSinks[i] << std::endl;
				}
				break;
			}
			//create a distribution object 
			std::uniform_int_distribution<int> sinkDist(firstNextRowSpace, getTotalSpaces()+1);
			int sink;
			do{
				sink = sinkDist(generator);
			}while(takenSinks[sink]);
			takenSinks[sink] = true;
			ladders.push_back(std::make_tuple(space, sink));
		}
	}

	//constructor cleanup
	delete [] takenSinks;
}

Board::~Board(){}

/** Getter function definitions */
int Board::getSize(){
	return size;
}
std::vector<std::tuple<int,int>> Board::getLadders(){
	return ladders;
}

/** Setter function definitions */
void Board::setSize(int s){
	s = size;
}
/** dynamically calcuate the total number of spaces on the board */
int Board::getTotalSpaces(){
	return size*size;
}

/** Overload the << operator to easily print the board */
std::ostream& operator<<(std::ostream &os, const Board& board) {
	os << "Board size is: " << board.size;
	return os;
}