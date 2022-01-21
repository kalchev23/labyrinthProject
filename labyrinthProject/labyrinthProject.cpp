
#include <iostream>
#include <vector>

using namespace std;

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

struct Dir {

	Dir() :Dir(false, false, false, false) {

	}

	Dir(bool east, bool west, bool north, bool south) {
		East = east;
		West = west;
		North = north;
		South = south;
	}

	bool East = false;
	bool West = false;
	bool North = false;
	bool South = false;

	bool Any() {
		return East || West || South || North;
	}
};

int main()
{
	srand(time(0));

	char notVisitedSymbols[20][80] = {};
	int visitedSymbols[20][80] = {};


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

	notVisitedSymbols[1][1] = 'S';
	notVisitedSymbols[18][78] = 'E';
	visitedSymbols[1][1] = 1;

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			cout << visitedSymbols[i][j];
		}
		cout << endl;
	}

	int turns = 0;
	int counter = 0;
	int i = 1, j = 1;
	int index = 0;
	bool isBackTracking = false;
	const int maxWestTurns = 1;
	const int maxNorthTurns = 1;
	int northTurns = 0;
	int westTurns = 0;

	vector<Cell> curPos = { Cell(1,1,1) };
	vector<Dir> dirs = {};
	vector<int> steps = { -1 };

	while (i != 18 || j != 78)
	{
		counter++;

		//1-left,2-right,3-top,4-down

		vector<int> neighbours;

		if (isBackTracking && dirs.size() > 0) {
			Dir last = *(dirs.end() - 1);
			if (last.West) {
				neighbours.push_back(1);
			}
			if (last.East) {
				neighbours.push_back(2);
			}
			if (last.North) {
				neighbours.push_back(3);
			}
			if (last.South) {
				neighbours.push_back(4);
			}
		}
		else {
			dirs.push_back(Dir());
			//West dir possible
			if (j > 1 && visitedSymbols[i][j - 2] == 0 && (visitedSymbols[i + 1][j - 1] == 0 || i == 18) && (visitedSymbols[i - 1][j - 1] == 0 || i == 1))
			{
				neighbours.push_back(1);
				(*(dirs.end() - 1)).West = true;
			}

			//East dir possible
			if (steps.size() > 0 && (*(steps.end() - 1) == 3 || *(steps.end() - 1) == 4))
			{
				j++;
			}
			if (j < 78 && visitedSymbols[i][j + 2] == 0 && (visitedSymbols[i + 1][j + 1] == 0 || i == 18) && (visitedSymbols[i - 1][j + 1] == 0 || i == 1))
			{
				neighbours.push_back(2);
				(*(dirs.end() - 1)).East = true;
			}
			if (steps.size() > 0 && (*(steps.end() - 1) == 3 || *(steps.end() - 1) == 4))
			{
				j--;
			}

			//North dir possible
			if (steps.size() > 0 && (*(steps.end() - 1) == 1))
			{
				j--;
			}
			if (i > 1 && visitedSymbols[i - 2][j] == 0 && (visitedSymbols[i - 1][j - 1] == 0 || j == 1) && (visitedSymbols[i - 1][j + 1] == 0 || j == 78))
			{
				// <-
				if (steps.size() > 1 && *(steps.end() - 1) == 1 && *(steps.end() - 2) == 1)
				{
					if (visitedSymbols[i - 2][j + 1] == 0 && visitedSymbols[i - 1][j + 1] == 0 && (visitedSymbols[i - 1][j + 2] == 0 || i == 18)) {
						neighbours.push_back(3);
						(*(dirs.end() - 1)).North = true;
					}
				}
				// ->    ^
				else
				{
					if (visitedSymbols[i - 2][j + 1] == 0 && visitedSymbols[i - 1][j + 1] == 0 && (j == 77 || visitedSymbols[i - 1][j + 2] == 0))
					{
						neighbours.push_back(3);
						(*(dirs.end() - 1)).North = true;
					}
				}
			}
			if (steps.size() > 0 && (*(steps.end() - 1) == 1))
			{
				j++;
			}

			//South dir possible
			if (steps.size() > 0 && (*(steps.end() - 1) == 2))
			{
				j--;
			}
			if (i < 18 && visitedSymbols[i + 2][j] == 0 && (visitedSymbols[i + 1][j - 1] == 0 || j == 1) && (visitedSymbols[i + 1][j + 1] == 0 || j == 78))
			{
				if (steps.size() > 1 && *(steps.end() - 1) == 2 && *(steps.end() - 2) == 2)
				{
					if (visitedSymbols[i + 1][j + 2] == 0 && visitedSymbols[i + 2][j + 1] == 0 && (visitedSymbols[i + 1][j + 1] == 0 || i == 1)) {
						neighbours.push_back(4);
						(*(dirs.end() - 1)).South = true;
					}
				}
				else
				{
					if (visitedSymbols[i + 2][j + 1] == 0 && visitedSymbols[i + 1][j + 1] == 0 && (visitedSymbols[i + 1][j + 2] == 0)) {
						neighbours.push_back(4);
						(*(dirs.end() - 1)).South = true;
					}
				}
			}
			if (steps.size() > 0 && (*(steps.end() - 1) == 2))
			{
				j++;
			}
		}

		vector<int> possibleDir;
		for (int m = 0; m < neighbours.size(); m++)
		{
			if (neighbours[m] == 1 && westTurns < maxWestTurns) {
				possibleDir.push_back(1);
			}
			else if (neighbours[m] == 3 && northTurns < maxNorthTurns) {
				possibleDir.push_back(3);
			}
			else if (neighbours[m] == 2 || neighbours[m] == 4) {
				possibleDir.push_back(neighbours[m]);
			}
		}

		if (!possibleDir.empty())
		{
			int a = neighbours.size();
			int ind = rand() % a;
			int nextCellDir = neighbours[ind];

			//cout << nextCellDir << endl;
			switch (nextCellDir)
			{
			case 1: (*(dirs.end() - 1)).West = false; westTurns++; break;
			case 2: (*(dirs.end() - 1)).East = false; break;
			case 3: (*(dirs.end() - 1)).North = false; northTurns++; break;
			case 4: (*(dirs.end() - 1)).South = false; break;
			}

			isBackTracking = false;
			if (steps.size() > 0 && (*(steps.end() - 1) != nextCellDir))
			{
				turns++;
			}

			switch (nextCellDir)
			{
			case 1:
				index++;
				curPos.push_back(Cell(1, j - 1, i));
				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				j--;
				break;

			case 2:
				if (steps.size() > 0 && (*(steps.end() - 1) == 3 || *(steps.end() - 1) == 4)) {
					j++;
				}
				index++;
				curPos.push_back(Cell(1, j + 1, i));
				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				j++;
				break;

			case 3:
				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				if (steps.size() > 0 && *(steps.end() - 1) == 1)
				{
					curPos.push_back(Cell(1, j - 1, i, true));
					j--;
				}
				else {
					curPos.push_back(Cell(1, j + 1, i, true));
				}

				index++;
				visitedSymbols[curPos[index].y][curPos[index].x] = 1;

				index++;
				curPos.push_back(Cell(2, j, i - 1, false, j + 1, i - 1));
				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				visitedSymbols[curPos[index].y1][curPos[index].x1] = 1;
				i--;
				break;

			case 4:
				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				if (steps.size() > 0 && *(steps.end() - 1) == 2)
				{
					curPos.push_back(Cell(1, j - 1, i, true));
					j--;
				}
				else {
					curPos.push_back(Cell(1, j + 1, i, true));
				}
				index++;
				visitedSymbols[curPos[index].y][curPos[index].x] = 1;

				index++;
				curPos.push_back(Cell(2, j, i + 1, false, j + 1, i + 1));

				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				visitedSymbols[curPos[index].y1][curPos[index].x1] = 1;
				i++;
				break;
			}

			steps.push_back(nextCellDir);
		}
		else
		{
			isBackTracking = true;
			//Backtracking
			do {
				visitedSymbols[curPos[index].y][curPos[index].x] = 0;
				if (curPos[index].width == 2)
				{
					visitedSymbols[curPos[index].y1][curPos[index].x1] = 0;
				}
				curPos.pop_back();
				index--;

				if (index > -1 && curPos[index].isDummyTurn) {
					visitedSymbols[curPos[index].y][curPos[index].x] = 0;
					curPos.pop_back();
					index--;
				}

				if (index > -1) {
					i = curPos[index].y;
					j = curPos[index].x;
				}
				else {
					i = j = 1;
				}

				if (*(steps.end() - 1) == 1) {
					westTurns--;
				}
				else if (*(steps.end() - 1) == 3) {
					northTurns--;
				}

				steps.pop_back();
				dirs.pop_back();
			} while (!(*(dirs.end() - 1)).Any());
		}

		if (counter % 200000 == 0) {
			cout << endl;
			cout << endl;

			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					cout << visitedSymbols[i][j];
				}
				cout << endl;
			}
		}
	}

	cout << endl;
	cout << endl;

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			cout << visitedSymbols[i][j];
		}
		cout << endl;
	}

	return 0;
}
