
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

	int counter = 0;
	int i = 1, j = 1;
	int prevChoice = 0;
	int index = 0;

	Cell currentCell;
	currentCell.width = 1;
	currentCell.x = 1;
	currentCell.y = 1;
	curPos.push_back(currentCell);

	while (i != 18 || j != 78)
	{
		counter++;
		if (counter > 100)
			break;

		//1-left,2-right,3-top,4-down
		int choice = rand() % 4 + 1;

		if (choice == 1)
		{
			if (prevChoice == 3 || prevChoice == 4)
				j--;
			if (visitedSymbols[i][j + 2] == 0 && (visitedSymbols[i + 1][j + 1] == 0 || i == 18) && (visitedSymbols[i - 1][j + 1] == 0 || i == 1))
			{
				index++;
				curPos[index].width = 1;
				curPos[index].x = j - 1;
				curPos[index].y = i;
				curPos.push_back(curPos[index]);

				visitedSymbols[curPos[index].x][curPos[index].y] = 1;

				j--;
			}
			else
			{
				continue;
			}
		}
		if (choice == 2)
		{
			if (prevChoice == 3 || prevChoice == 4)
				j++;
			if (visitedSymbols[i][j + 2] == 0 && (visitedSymbols[i + 1][j + 1] == 0 || i == 18) && (visitedSymbols[i - 1][j + 1] == 0 || i == 1))
			{
				index++;
				curPos[index].width = 1;
				curPos[index].x = j + 1;
				curPos[index].y = i;
				curPos.push_back(curPos[index]);

				visitedSymbols[curPos[index].x][curPos[index].y] = 1;

				j++;
			}
		}
		if (choice == 3)
		{
			if (prevChoice != 3 && visitedSymbols[i][j + 2] == 0 && visitedSymbols[i - 1][j + 1] == 0 && (visitedSymbols[i + 1][j + 1] == 0 || i == 18))
			{
				curPos[index].width = 2;
				curPos[index].x1 = j + 1;
				curPos[index].y1 = i;
				curPos.push_back(curPos[index]);

				visitedSymbols[curPos[index].x1][curPos[index].y1] = 1;
			}
			else if (visitedSymbols[i - 1][j - 1] == 0 && visitedSymbols[i - 2][j] == 0 && visitedSymbols[i - 1][j + 1] == 0)
			{
				index++;
				curPos[index].width = 2;
				curPos[index].x = j;
				curPos[index].y = i - 1;
				curPos[index].x1 = j + 1;
				curPos[index].y1 = i - 1;
				curPos.push_back(curPos[index]);

				visitedSymbols[curPos[index].x][curPos[index].y] = 1;
				visitedSymbols[curPos[index].x1][curPos[index].y1] = 1;

				i--;
			}
		}
		if (choice == 4)
		{
			if (prevChoice != 4 && visitedSymbols[i][j + 2] == 0 && visitedSymbols[i + 1][j + 1] == 0 && (visitedSymbols[i - 1][j + 1] == 0 || i == 1))
			{
				curPos[index].width = 2;
				curPos[index].x1 = j + 1;
				curPos[index].y1 = i;
				curPos.push_back(curPos[index]);
			}
			else if (visitedSymbols[i + 1][j - 1] == 0 && visitedSymbols[i + 2][j] == 0 && visitedSymbols[i + 1][j + 1] == 0)
			{
				index++;
				curPos[index].width = 2;
				curPos[index].x = j;
				curPos[index].y = i + 1;
				curPos[index].x1 = j + 1;
				curPos[index].y1 = i + 1;
				curPos.push_back(curPos[index]);

				visitedSymbols[curPos[index].x][curPos[index].y] = 1;
				visitedSymbols[curPos[index].x1][curPos[index].y1] = 1;

				i++;
			}
		}
		prevChoice = choice;
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
