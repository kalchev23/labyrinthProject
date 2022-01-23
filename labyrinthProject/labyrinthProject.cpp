/**
*
* Solution to course project # 10
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2021/2022
*
* @author Daniel Kalchev
* @idnumber 5MI0600009
* @compiler VC
*
* Maze generating program
*
*/

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

enum Directions 
{
	Initial = 0,//0000
	West = 1,   //0001
	East = 2,   //0010
	South = 4,  //0100
	North = 8   //1000
};

struct Cell
{
	Cell(int width, int x, int y, bool isDummyTurn = false, int x1 = -1, int y1 = -1) 
	{
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

const int boardRows = 20;
const int boardCols = 80;

void initializeMaze(int visitedSymbols[][boardCols]) 
{
	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (i == 0 || i == boardRows - 1 || j == 0 || j == boardCols - 1)
			{
				visitedSymbols[i][j] = 2;
			}
			else 
			{
				visitedSymbols[i][j] = 0;
			}
		}
	}

	visitedSymbols[1][1] = 1;
}

void printMaze(int visitedSymbols[][boardCols]) 
{
	const char wall = 219;

	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			// Custom task appearance -> for better readability
			if (i == 1 && j == 0) 
			{
				cout << "S ";
				j++;
			}
			else if (i == 18 && j == 78) 
			{
				cout << " E";
				j++;
			}
			else if (visitedSymbols[i][j] == 1) 
			{
				cout << " ";
			}
			else 
			{
				cout << wall;
			}

			// Original task appearance
			/*
			if (i == 1 && j == 1) {
				cout << 'S';
			}
			else if (i == 18 && j == 78) {
				cout << 'E';
			}
			else if (i == 0 || i == 19)
			{
				cout<< '-';
			}
			else if (j == 0 || j == 79)
			{
				cout<< '|';
			}
			else if (visitedSymbols[i][j] == 1) {
				cout << ' ';
			}
			else
			{
				cout<< '#';
			}
			*/
		}
		cout << endl;
	}
}

void saveMaze(int visitedSymbols[][boardCols]) 
{
	fstream file;
	file.open("maze.txt", ios::out);
	if (!file.is_open()) 
	{
		return;
	}

	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (i == 1 && j == 1) 
			{
				file << 'S';
			}
			else if (i == 18 && j == 78) 
			{
				file << 'E';
			}
			else if (i == 0 || i == 19)
			{
				file << '-';
			}
			else if (j == 0 || j == 79)
			{
				file << '|';
			}
			else if (visitedSymbols[i][j] == 1) 
			{
				file << ' ';
			}
			else
			{
				file << '#';
			}
		}
		file << endl;
	}
}

void copyMatrix(int source[][80], int destination[][boardCols]) 
{
	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			destination[i][j] = source[i][j];
		}
	}
}

Directions removeDirection(Directions initial, Directions toRemove) 
{
	int dirValue = 0;

	switch (toRemove)
	{
	case Directions::West:
		dirValue = 0;
		break;
	case Directions::East:
		dirValue = 1;
		break;
	case Directions::South:
		dirValue = 2;
		break;
	case Directions::North:
		dirValue = 3;
		break;
	}

	return (Directions)(initial & ~(1 << dirValue));
}

Directions getPossibleDirections(int visitedSymbols[][80], vector<Directions>& chosenDirections, int i, int j) 
{
	Directions result = Directions::Initial;

	//West dir possible
	if (j > 1 && (visitedSymbols[i][j - 2] == 0 || j == 2) && (visitedSymbols[i + 1][j - 1] == 0 || i == 18) && (visitedSymbols[i - 1][j - 1] == 0 || i == 1))
	{
		result = (Directions)(result | Directions::West);
	}

	//East dir possible
	if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::North || chosenDirections.back() == Directions::South))
	{
		j++;
	}
	if (j < 78 && (visitedSymbols[i][j + 2] == 0 || j == 77) && (visitedSymbols[i + 1][j + 1] == 0 || i == 18) && (visitedSymbols[i - 1][j + 1] == 0 || i == 1))
	{
		result = (Directions)(result | Directions::East);
	}
	if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::North || chosenDirections.back() == Directions::South))
	{
		j--;
	}

	//North dir possible
	if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::West))
	{
		j--;
	}
	if (i > 1 && (visitedSymbols[i - 2][j] == 0 || i == 2) && (visitedSymbols[i - 1][j - 1] == 0 || j == 1) &&
		(visitedSymbols[i - 1][j + 1] == 0 || j == 78) && (visitedSymbols[i - 2][j + 1] == 0 || i == 2) &&
		(visitedSymbols[i - 1][j + 2] == 0 || j == 77))
	{
		if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::West))
		{
			if ((visitedSymbols[i][j - 1] == 0 || j == 1) && (visitedSymbols[i + 1][j] == 0 || i == 18))
			{
				result = (Directions)(result | Directions::North);
			}
		}
		else if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::East))
		{
			if ((visitedSymbols[i][j + 2] == 0 || j == 77) && (visitedSymbols[i + 1][j + 1] == 0 || i == 18))
			{
				result = (Directions)(result | Directions::North);
			}
		}
		else if (chosenDirections.size() > 0)
		{
			result = (Directions)(result | Directions::North);
		}
	}

	// South dir possible
	if (i < 18 && (visitedSymbols[i + 2][j] == 0 || i == 17) && (visitedSymbols[i + 1][j - 1] == 0 || j == 1) &&
		(visitedSymbols[i + 1][j + 1] == 0 || j == 78) && (visitedSymbols[i + 2][j + 1] == 0 || i == 17) &&
		(visitedSymbols[i + 1][j + 2] == 0 || j == 77))
	{

		if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::West))
		{
			if ((visitedSymbols[i][j - 1] == 0 || j == 1) && (visitedSymbols[i - 1][j] == 0 || i == 1))
			{
				result = (Directions)(result | Directions::South);
			}
		}
		else if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::East))
		{
			if ((visitedSymbols[i][j + 2] == 0 || j == 77) && (visitedSymbols[i - 1][j + 1] == 0 || i == 1))
			{
				result = (Directions)(result | Directions::South);
			}
		}
		else if (chosenDirections.size() > 0)
		{
			result = (Directions)(result | Directions::South);
		}
	}
	if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::West))
	{
		j++;
	}

	return result;
}

bool hasInvalidDirection(Directions direction) 
{
	return direction == Directions::Initial ||
		direction == Directions::North ||
		direction == Directions::West ||
		direction == (Directions)(Directions::North | Directions::West);
}

bool makePath(int visitedSymbols[][80], int startI, int startJ, vector<Cell>& currentPosition,
	int currentPositionLength = 1, int requiredLength = -1, bool byPass = false) 
{
	const int maxTries = 3 * requiredLength;
	int currentTries = 0;

	int i = startI, j = startJ;
	int currentPositionIndex = currentPositionLength - 1;
	bool isBackTracking = false;

	vector<Directions> possibleDirections = {};
	vector<Directions> chosenDirections = { Directions::Initial };

	while (true)
	{
		// Go to end
		if (requiredLength == -1) 
		{
			if (i == 18 && (j == 78 || (j == 77 && chosenDirections.back() == Directions::South))) 
			{
				return true;
			}
		}
		// Make path with required length
		else 
		{
			int steps = chosenDirections.size();
			if (steps - 1 >= requiredLength) 
			{
				return true;
			}

			if (currentTries >= maxTries) 
			{
				for (int k = currentPosition.size() - 1; k >= currentPositionLength; k--)
				{
					visitedSymbols[currentPosition[k].y][currentPosition[k].x] = 0;
					if (currentPosition[k].width == 2)
					{
						visitedSymbols[currentPosition[k].y1][currentPosition[k].x1] = 0;
					}
					currentPosition.pop_back();
				}

				return false;
			}
		}

		Directions currentPossibleDirections;

		if (isBackTracking && possibleDirections.size() > 1) 
		{
			currentPossibleDirections = possibleDirections.back();
		}
		else 
		{
			currentPossibleDirections = getPossibleDirections(visitedSymbols, chosenDirections, i, j);
			possibleDirections.push_back(currentPossibleDirections);
		}

		Directions check = currentPossibleDirections;

		if (!byPass) 
		{
			check = removeDirection(check, Directions::West);
			check = removeDirection(check, Directions::North);
		}
		byPass = false;

		if (check != Directions::Initial)
		{
			Directions chosenDir = Directions::Initial;
			int dirValue;
			do 
			{
				dirValue = rand() % 4 + 1;
				if (dirValue == 3)
				{
					dirValue = Directions::North;
				}
				chosenDir = (Directions)(currentPossibleDirections & dirValue);

			} while (chosenDir == Directions::Initial);

			possibleDirections.back() = removeDirection(possibleDirections.back(), chosenDir);

			isBackTracking = false;

			switch (chosenDir)
			{
			case Directions::West:
				currentPositionIndex++;
				currentPosition.push_back(Cell(1, j - 1, i));
				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				j--;
				break;

			case Directions::East:
				if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::North || chosenDirections.back() == Directions::South))
				{
					j++;
				}
				currentPositionIndex++;
				currentPosition.push_back(Cell(1, j + 1, i));
				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				j++;
				break;

			case Directions::North:
				if (chosenDirections.size() > 0 && chosenDirections.back() == Directions::West)
				{
					currentPosition.push_back(Cell(1, j - 1, i, true));
					j--;
					currentPositionIndex++;
					visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				}
				else if (chosenDirections.size() > 0 && chosenDirections.back() == Directions::East)
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
				if (chosenDirections.size() > 0 && chosenDirections.back() == Directions::West)
				{
					currentPosition.push_back(Cell(1, j - 1, i, true));
					j--;
					currentPositionIndex++;
					visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 1;
				}
				else if (chosenDirections.size() > 0 && (chosenDirections.back() == Directions::East || chosenDirections.back() == Directions::Initial))
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
			do 
			{
				if (currentPositionIndex <= currentPositionLength - 1) 
				{
					return false;
				}

				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 0;
				if (currentPosition[currentPositionIndex].width == 2)
				{
					visitedSymbols[currentPosition[currentPositionIndex].y1][currentPosition[currentPositionIndex].x1] = 0;
				}
				currentPosition.pop_back();
				currentPositionIndex--;

				if (currentPositionIndex > currentPositionLength - 1 && currentPosition[currentPositionIndex].isDummyTurn) 
				{
					visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 0;
					currentPosition.pop_back();
					currentPositionIndex--;
				}

				if (currentPositionIndex > currentPositionLength - 1) 
				{
					i = currentPosition[currentPositionIndex].y;
					j = currentPosition[currentPositionIndex].x;

				}
				else 
				{
					i = startI;
					j = startJ;
				}

				if (requiredLength != -1) 
				{
					if (chosenDirections.size() == 1 && hasInvalidDirection(possibleDirections.back())) 
					{
						return false;
					}
				}

				chosenDirections.pop_back();
				if (hasInvalidDirection(possibleDirections.back())) 
				{
					possibleDirections.pop_back();
				}

				currentTries++;
			} while (hasInvalidDirection(possibleDirections.back()));
		}
	}
}

void generateMaze(int visitedSymbols[][boardCols], int minimumPathLength = 35, int minimumBranchLength = 10, int maximumBranchLength = 60) 
{
	vector<Directions> dir = { Directions::Initial };

	const int maxTries = 150;
	int tries = maxTries + 1;

	while (true) {
		if (tries < maxTries) {
			break;
		}

		tries = 0;

		vector<Cell> currentPosition = { Cell(1,1,1) };
		initializeMaze(visitedSymbols);

		// Main path
		makePath(visitedSymbols, 1, 1, currentPosition);
		int mainPathLength = currentPosition.size();

		int tempVisistedSymbols[boardRows][boardCols] = {};
		copyMatrix(visitedSymbols, tempVisistedSymbols);

		for (int i = 0; i < 5; i++)
		{
			int randomIndex;
			int length;

			do {
				copyMatrix(visitedSymbols, tempVisistedSymbols);
				do {
					do {
						randomIndex = rand() % (mainPathLength - 5) + 1;
					} while (currentPosition[randomIndex].isDummyTurn);

					length = rand() % (maximumBranchLength - minimumBranchLength + 1) + minimumBranchLength;
					if (randomIndex < minimumPathLength) {
						length += minimumPathLength - randomIndex;
					}

				} while (getPossibleDirections(tempVisistedSymbols, dir, currentPosition[randomIndex].y, currentPosition[randomIndex].x) == Directions::Initial);

				tries++;
				if (tries > maxTries) {
					break;
				}

			} while (!makePath(tempVisistedSymbols, currentPosition[randomIndex].y, currentPosition[randomIndex].x, currentPosition, currentPosition.size(), length, true));

			if (tries > maxTries) {
				break;
			}

			copyMatrix(tempVisistedSymbols, visitedSymbols);
		}
	}
}

int main()
{
	srand(time(0));

	int visitedSymbols[boardRows][boardCols] = {};

	const int minimumLengthFromStart = 35;
	const int minimumBranchLength = 10;
	const int maximumBranchLength = 50;

	generateMaze(visitedSymbols, minimumLengthFromStart, minimumBranchLength, maximumBranchLength);

	printMaze(visitedSymbols);

	saveMaze(visitedSymbols);

	return 0;
}