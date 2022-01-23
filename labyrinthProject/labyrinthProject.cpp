
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

enum Directions {
	Initial = 0,//0000
	West = 1,   //0001
	East = 2,   //0010
	South = 4,  //0100
	North = 8   //1000
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

void initializeBoard(int visitedSymbols[][80]) {

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (i == 0)
			{
				visitedSymbols[i][j] = 2;
			}
			else if (i == 19)
			{
				visitedSymbols[i][j] = 2;
			}
			else if (j == 0 || j == 79)
			{
				visitedSymbols[i][j] = 2;
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
	if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == Directions::North || *(chosenDirections.end() - 1) == Directions::South))
	{
		j++;
	}
	if (j < 78 && (visitedSymbols[i][j + 2] == 0 || j == 77) && (visitedSymbols[i + 1][j + 1] == 0 || i == 18) && (visitedSymbols[i - 1][j + 1] == 0 || i == 1))
	{
		result = (Directions)(result | Directions::East);
	}
	if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == Directions::North || *(chosenDirections.end() - 1) == Directions::South))
	{
		j--;
	}

	//North dir possible
	if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == Directions::West))
	{
		j--;
	}
	if (i > 1 && (visitedSymbols[i - 2][j] == 0 || i == 2) && (visitedSymbols[i - 1][j - 1] == 0 || j == 1) && (visitedSymbols[i - 1][j + 1] == 0 || j == 78))
	{
		if ((visitedSymbols[i - 2][j + 1] == 0 || i == 2) && (visitedSymbols[i - 1][j + 2] == 0 || j == 77))
		{
			if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == Directions::West))
			{
				if ((visitedSymbols[i][j - 1] == 0 || j == 1) && (visitedSymbols[i + 1][j] == 0 || i == 18))
				{
					result = (Directions)(result | Directions::North);
				}
			}
			else if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == Directions::East))
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

	}

	// South
	if (i < 18 && (visitedSymbols[i + 2][j] == 0 || i == 17) && (visitedSymbols[i + 1][j - 1] == 0 || j == 1) && (visitedSymbols[i + 1][j + 1] == 0 || j == 78))
	{
		if ((visitedSymbols[i + 2][j + 1] == 0 || i == 17) && (visitedSymbols[i + 1][j + 2] == 0 || j == 77))
		{
			if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == Directions::West))
			{
				if ((visitedSymbols[i][j - 1] == 0 || j == 1) && (visitedSymbols[i - 1][j] == 0 || i == 1))
				{
					result = (Directions)(result | Directions::South);
				}
			}
			else if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == Directions::East))
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

	}
	if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == Directions::West))
	{
		j++;
	}

	return result;
}

bool makePath(int visitedSymbols[][80], int startI, int startJ, vector<Cell>& currentPosition, int currentPositionLength = 1, int requiredLength = -1, bool byPass = false) {
	int turns = 0;
	int maxTries = 10;
	int currentTries = 0;
	int i = startI, j = startJ;
	int currentPositionIndex = currentPositionLength - 1;
	bool isBackTracking = false;

	vector<Directions> possibleDirections = {};
	vector<Directions> chosenDirections = { Directions::Initial };

	while (true)
	{
		// Go to end
		if (requiredLength == -1) {
			if (!(i != 18 || (j != 78 || (j != 77 && *(chosenDirections.end() - 1) == Directions::South)))) {
				return true;
			}
		}
		// Make path with required length
		else {
			int steps = chosenDirections.size();
			if (steps - 1 >= requiredLength) {
				return true;
			}

			if (currentTries >= maxTries) {
				return false;
			}
		}

		Directions currentPossibleDirections;

		if (isBackTracking && possibleDirections.size() > 1) {
			currentPossibleDirections = *(possibleDirections.end() - 1);
		}
		else {
			currentPossibleDirections = getPossibleDirections(visitedSymbols, chosenDirections, i, j);
			possibleDirections.push_back(currentPossibleDirections);
		}

		Directions check = currentPossibleDirections;

		if (!byPass) {
			check = (Directions)(check & ~(1 << 0));
			check = (Directions)(check & ~(1 << 3));
			byPass = false;
		}


		if (check != Directions::Initial)
		{
			Directions chosenDir = Directions::Initial;
			int ind;
			do {
				ind = rand() % 4 + 1; // 1-4
				if (ind == 3)
				{
					ind = 8;
				}
				chosenDir = (Directions)(currentPossibleDirections & ind);

			} while (chosenDir == Directions::Initial);

			switch (chosenDir)
			{
			case Directions::West:
				ind = 0;
				break;
			case Directions::East:
				ind = 1;
				break;
			case Directions::South:
				ind = 2;
				break;
			case Directions::North:
				ind = 3;
				break;
			}

			(*(possibleDirections.end() - 1)) = Directions(((*(possibleDirections.end() - 1)) & ~(1 << ind)));

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
				else if (chosenDirections.size() > 0 && (*(chosenDirections.end() - 1) == 2 || *(chosenDirections.end() - 1) == Directions::Initial))
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
			do {

				visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 0;
				if (currentPosition[currentPositionIndex].width == 2)
				{
					visitedSymbols[currentPosition[currentPositionIndex].y1][currentPosition[currentPositionIndex].x1] = 0;
				}
				currentPosition.pop_back();
				currentPositionIndex--;

				if (currentPositionIndex > currentPositionLength - 1 - 1 && currentPosition[currentPositionIndex].isDummyTurn) {
					visitedSymbols[currentPosition[currentPositionIndex].y][currentPosition[currentPositionIndex].x] = 0;
					currentPosition.pop_back();
					currentPositionIndex--;
				}

				if (currentPositionIndex > currentPositionLength - 1) {
					i = currentPosition[currentPositionIndex].y;
					j = currentPosition[currentPositionIndex].x;
				}
				else {
					i = startI;
					j = startJ;
				}

				if (chosenDirections.size() == 0) {
					return false;
				}

				chosenDirections.pop_back();
				possibleDirections.pop_back();
				++currentTries;

			} while (possibleDirections.size() > 1 && (*(possibleDirections.end() - 1)) == Directions::Initial);
		}
	}
}

void printMaze(int visitedSymbols[][80]) {
	char wall = 219;

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (i == 1 && j == 0) {
				cout << 'S';
			}
			else if (i == 18 && j == 79) {
				cout << 'E';
			}
			else if (visitedSymbols[i][j] == 1) {
				cout << " ";
			}
			else {
				cout << wall;
			}

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

void saveMaze(int visitedSymbols[][80]) {

	fstream file;
	file.open("maze.txt", ios::out);
	if (!file.is_open()) {
		return;
	}

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (i == 1 && j == 1) {
				file << 'S';
			}
			else if (i == 18 && j == 78) {
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
			else if (visitedSymbols[i][j] == 1) {
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


void copyMatrix(int source[][80], int destination[][80]) {
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			destination[i][j] = source[i][j];
		}
	}
}

void generateMaze(int visitedSymbols[20][80]) {

	initializeBoard(visitedSymbols);

	int tempVisistedSymbols[20][80] = {};
	vector<Cell> currentPosition = { Cell(1,1,1) };

	// Main path
	makePath(visitedSymbols, 1, 1, currentPosition);

	copyMatrix(visitedSymbols, tempVisistedSymbols);

	int mainPathLength = currentPosition.size();
	vector<Directions> dir;
	for (size_t i = 0; i < 5; i++)
	{
		int r;
		int length;
		do {
			copyMatrix(visitedSymbols, tempVisistedSymbols);
			do {
				do {
					r = rand() % (mainPathLength - 5);
					length = rand() % 50 + 6;
					if (r < 35) {
						length += 35 - r;
					}
				} while (currentPosition[r].isDummyTurn);

			} while (getPossibleDirections(tempVisistedSymbols, dir, currentPosition[r].y, currentPosition[r].x) == Directions::Initial /*|| getPossibleDirections(tempVisistedSymbols, dir, currentPosition[r].y, currentPosition[r].x) == Directions::North || getPossibleDirections(tempVisistedSymbols, dir, currentPosition[r].y, currentPosition[r].x) == Directions::West || getPossibleDirections(tempVisistedSymbols, dir, currentPosition[r].y, currentPosition[r].x) == (Directions::West | Directions::North)*/);

		} while (!makePath(tempVisistedSymbols, currentPosition[r].y, currentPosition[r].x, currentPosition, currentPosition.size(), length, true));

		copyMatrix(tempVisistedSymbols, visitedSymbols);
	}
}

int main()
{
	srand(time(0));

	int visitedSymbols[20][80] = {};

	generateMaze(visitedSymbols);

	printMaze(visitedSymbols);
	saveMaze(visitedSymbols);

	return 0;
}