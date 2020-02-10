#include "board.h"

/**
	Constructor for the board object
	@param s - the size of the board. Since the board is a 2d square, we only need one side length
	@param ds - the size of the dice
	@param ladderChance - a double between 0 (inc.) and 1(exc.) used to determine if any particular space
	is a ladder source

*/
Board::Board(int s, int ds, double ladderChance){
	/** Assign the private members */
	size = s;
	dicesize = ds;

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
				break;
			}
			//create a distribution object 
			std::uniform_int_distribution<int> sinkDist(firstNextRowSpace, getTotalSpaces());
			int sink;
			do{
				sink = sinkDist(generator);
			}while(takenSinks[sink]);
			takenSinks[sink] = true;
			int* ladderArr = new int[2];
			ladderArr[0] = space;
			ladderArr[1] = sink;
			ladders.push_back(ladderArr);
		}
	}

	//constructor cleanup
	delete [] takenSinks;
}

Board::~Board(){
	for(int i=0;i<ladders.size();i++){
		delete [] ladders.at(i);
	}
}

/** Getter function definitions */
int Board::getSize(){
	return size;
}
std::vector<int*> Board::getLadders(){
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

/** 
*	This method finds the optimal set of rolls to win the current board.
*	It is important to note that this algorithm is myopic, meaning it will only
*	look ahead to the next roll to determine which move will move it the furthest. 
*	To illustrate this, imagine the following scenario (dice size = 6):
*
*	The player has just started the game and is on space 0. Two ladders exist on the board:
		- One on space 5, which takes the player to space 40
		- One on space 7, which takes the player to space 90

*	The algorithm looks at spaces 1-6 for ladders, and finds the ladder on space 5. Since this 
*	ladder takes the player past space 6, the maximum "normal" roll, this is deemed the optimal roll,
*	and moves the piece here. On subsequent turns, the player moves 6 spaces until they reach space 100 on turn 11.
*	The actual optimal set of rolls for this board would be to move 6 on turn 1, bypassing the first ladder, and moving 1 on
*	the next turn, landing on the ladder on space 7, travelling all the way to space 90 on turn 2, and winning on turn 4. 
*/	
std::vector<int> Board::calculateOptimalRolls(){
	//start on the first space
	int curSpace = 0;
	std::vector<int> rolls;
	while(curSpace!=getTotalSpaces()){
		std::cout << "Starting loop; current space is " << curSpace << std::endl;
		//track the next space to end the turn, including after potentially following a ladder. Defaults to the space that is dicesize spaces away, i.e. the highest number of spaces one can move on a 'regular' roll of the die
		int destinationSpace = curSpace + dicesize;
		std::cout << "Default destinationSpace is " << destinationSpace << std::endl;
		//similarily, this variable tracks the number the player needs to roll to move to the next ideal space. defaults to the max number the player can roll, unless the player is near the end of the board
		int roll = dicesize;
		//if the end space is within the space of a normal roll, don't bother checking ladders, just roll and move to the end with the exact value.
		if(getTotalSpaces()-curSpace<=dicesize){
			std::cout << "End space in sight, moving to end space" << std::endl;
			roll = getTotalSpaces()-curSpace;
			rolls.push_back(roll);
			curSpace += roll;
			break;
		}
		std::cout << "Searching ladders" << std::endl;
		for(int i=0;i<ladders.size();i++){
			//if a ladder space is less than the maximum number of spaces away this turn, 
			int* ladder = ladders.at(i);
			int distance = ladder[0] - curSpace;
			std::cout << "[Ladder] distance to ladder at " << ladder[0] << ": " << distance << std::endl;
			//if the ladder is within move-range and the ladder destination is the furthest space seen so far, set this roll as the space to move.
			if((distance < dicesize && distance > 0) && (ladder[1]>destinationSpace)){
				std::cout << "[Ladder] Valid ladder found at space " << ladder[0];
				roll = distance;
				destinationSpace = ladder[1];
				std::cout << " Roll to move to this ladder is " << roll << " and ends on space " << destinationSpace << std::endl;
			}
		}
		std::cout << "Ideal roll is " << roll << ", next starting space is " << destinationSpace << std::endl;
		rolls.push_back(roll);
		curSpace = destinationSpace;
	}
	return rolls;

}

/** Overload the << operator to easily print the board */
std::ostream& operator<<(std::ostream &os, const Board& board) {
	os << "Board size is: " << board.size;
	return os;
}
