
#include <iostream>
#include <vector>

using namespace std;

struct Cell
{
	int width;
	int x, y;
	int x1, y1;
};

int main()
{
	vector<Cell> curPos;
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

	Cell currentCell;
	currentCell.width = 1;
	currentCell.x = 1;
	currentCell.y = 1;
	curPos.push_back(currentCell);

	vector<int> steps = { -1 };

	while (i != 18 || j != 78)
	{
		counter++;
		if (counter > 100)
			break;
		//1-left,2-right,3-top,4-down

		vector<int> neighbours;
		bool flagWest = false;

		//West dir possible
		if (*(steps.end() - 1) == 3 || *(steps.end() - 1) == 4)
		{
			j--;
			flagWest = true;
		}

		if (j > 1 && visitedSymbols[i][j - 2] == 0 && (visitedSymbols[i + 1][j - 1] == 0 || i == 18) && (visitedSymbols[i - 1][j - 1] == 0 || i == 1))
		{
			neighbours.push_back(1);
		}
		if (flagWest)
			j++;

		bool flagEast = false;
		//East dir possible
		if (*(steps.end() - 1) == 3 || *(steps.end() - 1) == 4)
		{
			j++;
			flagEast = true;
		}

		if (j < 78 && visitedSymbols[i][j + 2] == 0 && (visitedSymbols[i + 1][j + 1] == 0 || i == 18) && (visitedSymbols[i - 1][j + 1] == 0 || i == 1))
		{
			neighbours.push_back(2);
		}
		if (flagEast)
			j--;

		if (*(steps.end() - 1) == 1)
		{
			j--;
		}

		//North dir possible
		if (i > 1 && visitedSymbols[i - 2][j] == 0 && (visitedSymbols[i - 1][j - 1] == 0 || j == 1) && (visitedSymbols[i - 1][j + 1] == 0 || j == 78))
		{
			if (steps.size() > 1 && *(steps.end() - 1) == 1 && *(steps.end() - 2) == 1)
			{
				if (visitedSymbols[i - 2][j + 1] == 0 && visitedSymbols[i - 1][j + 1] == 0 && (visitedSymbols[i - 1][j + 2] == 0 || i == 18))
					neighbours.push_back(3);
			}
			else
			{
				if (visitedSymbols[i - 2][j + 1] == 0 && visitedSymbols[i - 1][j + 1] == 0 && (visitedSymbols[i - 1][j + 2] == 0 || j == 77))
					neighbours.push_back(3);
			}
		}

		//South dir possible
		if (i < 18 && visitedSymbols[i + 2][j] == 0 && (visitedSymbols[i + 1][j - 1] == 0 || j == 1) && (visitedSymbols[i + 1][j + 1] == 0 || j == 78))
		{
			if (steps.size() > 1 && *(steps.end() - 1) == 2 && *(steps.end() - 2) == 2)
			{
				if (visitedSymbols[i + 1][j + 2] == 0 && visitedSymbols[i + 2][j + 1] == 0 && (visitedSymbols[i + 1][j + 1] == 0 || i == 1))
					neighbours.push_back(4);
			}
			else
			{
				if (visitedSymbols[i + 2][j + 1] == 0 && visitedSymbols[i + 1][j + 1] == 0 && (visitedSymbols[i + 1][j + 2] == 0))
					neighbours.push_back(4);
			}
		}

		if (*(steps.end() - 1) == 1)
		{
			j++;
		}


		if (!neighbours.empty())
		{
			int d = neighbours.size();
			int r = rand() % d;
			int nextCellDir = neighbours[r];
			cout << nextCellDir << endl;
			if (*(steps.end() - 1) != nextCellDir)
			{
				turns++;
			}

			switch (nextCellDir)
			{
			case 1:
				index++;
				Cell _currentCell1;
				_currentCell1.width = 1;
				_currentCell1.x = j - 1;
				_currentCell1.y = i;
				curPos.push_back(_currentCell1);

				visitedSymbols[curPos[index].y][curPos[index].x] = 1;

				j--;
				break;
			case 2:
				if (*(steps.end() - 1) == 3 || *(steps.end() - 1) == 4)
					j++;
				index++;
				Cell _currentCell2;
				_currentCell2.width = 1;
				_currentCell2.x = j + 1;
				_currentCell2.y = i;
				curPos.push_back(_currentCell2);

				visitedSymbols[curPos[index].y][curPos[index].x] = 1;

				j++;
				break;
			case 3:
				if (*(steps.end() - 1) == 1)
				{
					j--;
					curPos[index].width = 2;
					curPos[index].x1 = curPos[index].x;
					curPos[index].y1 = curPos[index].y;
					curPos[index].x = j;
					curPos[index].y = i;
				}
				else {
					curPos[index].width = 2;
					curPos[index].x1 = j + 1;
					curPos[index].y1 = i;
				}

				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				visitedSymbols[curPos[index].y1][curPos[index].x1] = 1;

				index++;
				Cell _currentCell4;
				_currentCell4.width = 2;
				_currentCell4.x = j;
				_currentCell4.y = i - 1;
				_currentCell4.x1 = j + 1;
				_currentCell4.y1 = i - 1;
				curPos.push_back(_currentCell4);

				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				visitedSymbols[curPos[index].y1][curPos[index].x1] = 1;

				i--;
				break;
			case 4:
				if (*(steps.end() - 1) == 2)
				{
					j--;
					curPos[index].width = 2;
					curPos[index].x1 = curPos[index].x;
					curPos[index].y1 = curPos[index].y;
					curPos[index].x = j;
					curPos[index].y = i;
				}
				else {
					curPos[index].width = 2;
					curPos[index].x1 = j + 1;
					curPos[index].y1 = i;
				}

				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				visitedSymbols[curPos[index].y1][curPos[index].x1] = 1;

				index++;
				Cell _currentCell6;
				_currentCell6.width = 2;
				_currentCell6.x = j;
				_currentCell6.y = i + 1;
				_currentCell6.x1 = j + 1;
				_currentCell6.y1 = i + 1;
				curPos.push_back(_currentCell6);

				visitedSymbols[curPos[index].y][curPos[index].x] = 1;
				visitedSymbols[curPos[index].y1][curPos[index].x1] = 1;

				i++;
				break;
			}

			steps.push_back(nextCellDir);
		}
		else
		{
			//Backtracking
			if (curPos[index].width == 1)
			{
				visitedSymbols[curPos[index].y][curPos[index].x] = 0;
			}
			else if (curPos[index].width == 2)
			{
				visitedSymbols[curPos[index].y][curPos[index].x] = 0;
				visitedSymbols[curPos[index].y1][curPos[index].x1] = 0;
			}
			curPos.pop_back();
			index--;
			i = curPos[index].y;
			j = curPos[index].x;

			steps.pop_back();
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
