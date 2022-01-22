
#include <iostream>
#include <vector>

using namespace std;

enum Directions {
	Initial = 0,//0000
	West = 1,//0001
	East = 2,//0010
	South = 4,//0100
	North = 8// 1000
};

struct Cell
{
	Cell(int width, int x, int y, bool isDummyTurn = false, int x1 = -1, int y1 = -1) {
		this->width = width;
		this->x = x;
		this->y = y;
		this->x1 = x1;
		this->y1 = y1;
		this->isDummyTurn = isDummyTurn;
	}

	int width;
	int x, y;
	int x1, y1;
	bool isDummyTurn = false;
};

void printA(int visitedSymbols[][80]) {
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			cout << visitedSymbols[i][j];
		}
		cout << endl;
	}
	cout << endl;
	cout << endl;
}

void initializeBoard(char notVisitedSymbols[][80], int visitedSymbols[][80]) {

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (i == 0 || i == 19)
			{
				notVisitedSymbols[i][j] = '-';
				visitedSymbols[i][j] = 2;
			}
			else if (j == 0 || j == 79)
			{
				notVisitedSymbols[i][j] = '|';
				visitedSymbols[i][j] = 2;
			}
			else
			{
				notVisitedSymbols[i][j] = '#';
			}
		}
	}
	visitedSymbols[1][1] = 1;
}

Directions getPossibleDirections(int visitedSymbols[][80], vector<Directions>& chosenDirections, int i, int j) {
	Directions result = Directions::Initial;
	//West dir possible
	if (j > 1 && (visitedSymbols[i][j - 2] == 0 || j == 2) && (visitedSymbols[i + 1][j - 1] == 0 || i == 18) && (visitedSymbols[i - 1][j - 1] == 0 || i == 1))
	{
		result = (Directions)(result | Directions::West);
	}

	//East dir possible
	if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 3 || *(chosenDirections.end() - 1) == 4))
	{
		j++;
	}
	//change 2nd condition
	if (j < 78 && (visitedSymbols[i][j + 2] == 0 || j == 77) && (visitedSymbols[i + 1][j + 1] == 0 || i == 18) && (visitedSymbols[i - 1][j + 1] == 0 || i == 1))
	{
		result = (Directions)(result | Directions::East);
	}
	if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 3 || *(chosenDirections.end() - 1) == 4))
	{
		j--;
	}

	//North dir possible
	if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 1))
	{
		j--;
	}
	if (i > 1 && (visitedSymbols[i - 2][j] == 0 || i == 2) && (visitedSymbols[i - 1][j - 1] == 0 || j == 1) && (visitedSymbols[i - 1][j + 1] == 0 || j == 78))
	{
		if ((visitedSymbols[i - 2][j + 1] == 0 || i == 2) && (visitedSymbols[i - 1][j + 2] == 0 || j == 77))
		{
			if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 1))
			{
				if ((visitedSymbols[i][j - 1] == 0 || j == 1) && (visitedSymbols[i + 1][j] == 0 || i == 18))
				{
					result = (Directions)(result | Directions::North);
				}
			}
			else if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 2))
			{
				if ((visitedSymbols[i][j + 1] == 0 || j == 77) && (visitedSymbols[i + 1][j] == 0 || i == 18))
				{
					result = (Directions)(result | Directions::North);
				}
			}
			else if (chosenDirections.size() > 0)
			{
				result = (Directions)(result | Directions::North);
			}
		}

	}

	// South
	if (i < 18 && (visitedSymbols[i + 2][j] == 0 || i == 17) && (visitedSymbols[i + 1][j - 1] == 0 || j == 1) && (visitedSymbols[i + 1][j + 1] == 0 || j == 78))
	{
		if ((visitedSymbols[i + 2][j + 1] == 0 || i == 17) && (visitedSymbols[i + 1][j + 2] == 0 || j == 77))
		{
			if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 1))
			{
				if ((visitedSymbols[i][j - 1] == 0 || j == 1) && (visitedSymbols[i - 1][j] == 0 || i == 1))
				{
					result = (Directions)(result | Directions::South);
				}
			}
			else if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 2))
			{
				if ((visitedSymbols[i][j + 1] == 0 || j == 77) && (visitedSymbols[i - 1][j] == 0 || i == 1))
				{
					result = (Directions)(result | Directions::South);
				}
			}
			else if (chosenDirections.size() > 0)
			{
				result = (Directions)(result | Directions::South);
			}

		}

	}
	if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 1))
	{
		j++;
	}

	return result;
}

void makeMainPath(int visitedSymbols[][80]) 
{
	int turns = 0;
	int i = 1, j = 1;
	int currentPositionIndex = 0;
	bool isBackTracking = false;
	const int maxWestTurns = 1;
	const int maxNorthTurns = 1;
	int northTurns = 0;
	int westTurns = 0;

	vector<Cell> currentPosition = { Cell(1,1,1) };
	vector<Directions> possibleDirections = {};
	vector<Directions> chosenDirections = { Directions::Initial };

	while (i != 18 || j != 78)
	{
		//printA(visitedSymbols);
		//1-left,2-right,3-top,4-down
		Directions currentPossibleDirections;

		if (isBackTracking && possibleDirections.size() > 1) {
			currentPossibleDirections = *(possibleDirections.end() - 1);
		}
		else {
			currentPossibleDirections = getPossibleDirections(visitedSymbols, chosenDirections, i, j);
			possibleDirections.push_back(currentPossibleDirections);
		}
		//cout << ((int)(currentPossibleDirections&Directions::North)>0 ?"North":"") << endl;

		if (westTurns >= maxWestTurns) {
			currentPossibleDirections = (Directions)(currentPossibleDirections & ~(1));
		}
		if (northTurns >= maxNorthTurns) {
			currentPossibleDirections = (Directions)(currentPossibleDirections & ~(1 << 3));
		}

		if (currentPossibleDirections != Directions::Initial)
		{
			Directions chosenDir = Directions::Initial;
			int ind;
			do {
				ind = rand() % 4 + 1;// 1-4
				if (ind == 3)
				{
					ind = 8;
				}
				chosenDir = (Directions)(currentPossibleDirections & ind);

			} while (chosenDir == Directions::Initial);

			//cout << ((chosenDir == Directions::North) ? "North" : "") << endl;
			
		// TODO: fix
			int invertOfParts = 0b1111 - chosenDir;
			(*(possibleDirections.end() - 1)) = Directions(((*(possibleDirections.end() - 1)) & ~invertOfParts));
			if (chosenDir == Directions::North) {
				cout << "Not north: " << ~chosenDir << endl;
				cout << *(possibleDirections.end() - 1) << endl;
				//cout << "NN: " << ((*(possibleDirections.end() - 1)) && Directions::North) << endl;
			}

			if (chosenDir == Directions::North) {
				northTurns++;
			}
			else if (chosenDir == Directions::West) {
				westTurns++;
			}

			isBackTracking = false;
			/*	if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) != nextCellDir))
				{
					turns++;
				}*/

			switch (chosenDir)
			{
			case Directions::West:
				currentPositionIndex++;
				currentPosition.push_back(Cell(1, j - 1, i));
				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				j--;
				break;

			case Directions::East:
				if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 3 || *(chosenDirections.end() - 1) == 4))
				{
					j++;
				}
				currentPositionIndex++;
				currentPosition.push_back(Cell(1, j + 1, i));
				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				j++;
				break;

			case Directions::North:
				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				if (chosenDirections.size() > 0 && *(chosenDirections.end() - 1) == 1)
				{
					currentPosition.push_back(Cell(1, j - 1, i, true));
					j--;
					currentPositionIndex++;
					visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				}
				else if (chosenDirections.size() > 0 && *(chosenDirections.end() - 1) == 2)
				{
					currentPosition.push_back(Cell(1, j + 1, i, true));
					currentPositionIndex++;
					visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				}

				currentPositionIndex++;
				currentPosition.push_back(Cell(2, j, i - 1, false, j + 1, i - 1));
				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				visitedSymbols[currentPosition[currentPositionIndex].y1][currentPosition[currentPositionIndex].x1] = 1;
				i--;
				break;

			case Directions::South:
				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				if (chosenDirections.size() > 0 && *(chosenDirections.end() - 1) == 1)
				{
					currentPosition.push_back(Cell(1, j - 1, i, true));
					j--;
					currentPositionIndex++;
					visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				}
				else if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 2 || *(chosenDirections.end() - 1) == -1))
				{
					currentPosition.push_back(Cell(1, j + 1, i, true));
					currentPositionIndex++;
					visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				}

				currentPositionIndex++;
				currentPosition.push_back(Cell(2, j, i + 1, false, j + 1, i + 1));

				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				visitedSymbols[currentPosition[currentPositionIndex].y1][currentPosition[currentPositionIndex].x1] = 1;
				i++;
				break;
			}

			chosenDirections.push_back(chosenDir);
		}
		else
		{
			isBackTracking = true;
			//Backtracking
			do {

				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 0;
				if (currentPosition[currentPositionIndex].width == 2)
				{
					visitedSymbols[currentPosition[currentPositionIndex].y1][currentPosition[currentPositionIndex].x1] = 0;
				}
				currentPosition.pop_back();
				currentPositionIndex--;

				if (currentPositionIndex > -1 && currentPosition[currentPositionIndex].isDummyTurn) {
					visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 0;
					currentPosition.pop_back();
					currentPositionIndex--;
				}

				if (currentPositionIndex > -1) {
					i = currentPosition[currentPositionIndex].y;
					j = currentPosition[currentPositionIndex].x;
				}
				else {
					i = j = 1;
				}

				if (*(chosenDirections.end() - 1) == Directions::West) {
					westTurns--;
				}
				else if (*(chosenDirections.end() - 1) == Directions::North) {
					northTurns--;
				}

				//cout << "Step : " << *(chosenDirections.end() - 1) << "  i : " << i << "  j : " << j << endl;

				chosenDirections.pop_back();
				possibleDirections.pop_back();
			} while (possibleDirections.size() > 1 && (*(possibleDirections.end() - 1)) == Directions::Initial);
		}

		/*if (counter > 100000)
			break;*/
	}
}

void printMaze(char notVisitedSymbols[][80], int visitedSymbols[][80]) {
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (i == 1 && j == 1)
			{
				notVisitedSymbols[1][1] = 'S';
			}
			else if (i == 18 && j == 78)
			{
				notVisitedSymbols[18][78] = 'E';
			}
			else if (visitedSymbols[i][j] == 1)
			{
				notVisitedSymbols[i][j] = ' ';
			}
			cout << notVisitedSymbols[i][j];
		}
		cout << endl;
	}
}

int main()
{
	srand(time(0));

	char notVisitedSymbols[20][80] = {};
	int visitedSymbols[20][80] = {};
	initializeBoard(notVisitedSymbols, visitedSymbols);

	/**/
	makeMainPath(visitedSymbols);

	//makeOtherPaths(visitedSymbols);

	printMaze(notVisitedSymbols, visitedSymbols);

	return 0;
}