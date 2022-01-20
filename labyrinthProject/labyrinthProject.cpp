
#include <iostream>

using namespace std;

void SecondColumnWidth(int visitedSymbols[][80], char notVisitedSymbols[][80], int &i,int &j,bool &isLeftDependent, bool &isRightDependent, int &prevChoice)
{
	if (visitedSymbols[i][j - 1] == 1 && visitedSymbols[i][j + 1] == 1)
	{
		int r = rand() % 2;
		if (r == 0)
		{
			notVisitedSymbols[i][j - 1] = ' ';
			isLeftDependent = true;
		}
		else
		{
			notVisitedSymbols[i][j + 1] = ' ';
			isRightDependent = true;
		}
	}
	else if (visitedSymbols[i][j - 1] == 2 && visitedSymbols[i][j + 1] == 1)
	{
		notVisitedSymbols[i][j + 1] = ' ';
		isRightDependent = true;
	}
	else if (visitedSymbols[i][j - 1] == 1 && visitedSymbols[i][j + 1] == 2)
	{
		notVisitedSymbols[i][j - 1] = ' ';
		isLeftDependent = true;
	}

	if (prevChoice == 1 && visitedSymbols[i][j - 1] == 0)
	{
		visitedSymbols[i][j - 1] = 1;
		notVisitedSymbols[i][j - 1] = ' ';
		isLeftDependent = true;
	}
	if (prevChoice == 2 && visitedSymbols[i][j + 1] == 0)
	{
		visitedSymbols[i][j + 1] = 1;
		notVisitedSymbols[i][j + 1] = ' ';
		isRightDependent = true;
	}
	if (isLeftDependent)
	{
		visitedSymbols[i][j - 1] = 1;
		notVisitedSymbols[i][j - 1] = ' ';
	}
	if (isRightDependent)
	{
		visitedSymbols[i][j + 1] = 1;
		notVisitedSymbols[i][j + 1] = ' ';
	}
}

int main()
{
    char notVisitedSymbols[20][80] = {};
	int visitedSymbols[20][80] = {0};
	
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (i == 0 || i == 19)
			{
				notVisitedSymbols[i][j] = '-';
				visitedSymbols[i][j] = 2;
			}
			else if(j == 0 || j == 79)
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

	int prevChoice = 0;
	bool isLeftDependent = false;
	bool isRightDependent = true;
	int i = 1, j = 1;
	srand(time(0));
	int counter = 0;

	while (i != 18 || j != 78)
	{
		counter++;
		if (counter > 20)
			break;
		//1-left,2-right,3-top,4-down
		int choice = rand() % 4 + 1;
		cout << "choice = " << choice << endl;
		if (choice == 1)
		{
			isLeftDependent = false;
			isRightDependent = false;
			if (visitedSymbols[i][j - 1] == 0)
			{
				visitedSymbols[i - 1][j] = 2;
				visitedSymbols[i + 1][j] = 2;
				visitedSymbols[i][j - 1] = 1;
				notVisitedSymbols[i][j - 1] = ' ';
				j--;
			}
			else
				continue;
		}
		if (choice == 2)
		{
			isLeftDependent = false;
			isRightDependent = false;
			if (visitedSymbols[i][j + 1] == 0)
			{
				visitedSymbols[i - 1][j] = 2;
				visitedSymbols[i + 1][j] = 2;
				visitedSymbols[i][j + 1] = 1;
				notVisitedSymbols[i][j + 1] = ' ';
				j++;
			}
			else
				continue;
		}
		if (choice == 3)
		{
			if (visitedSymbols[i - 1][j] == 0)
			{
				visitedSymbols[i - 1][j] = 1;
				notVisitedSymbols[i - 1][j] = ' ';
				
				SecondColumnWidth(visitedSymbols, notVisitedSymbols, i, j, isLeftDependent, isRightDependent, prevChoice);

				i--;

				SecondColumnWidth(visitedSymbols, notVisitedSymbols, i, j, isLeftDependent, isRightDependent, prevChoice);
			}
			else
				continue;
		}
		if (choice == 4)
		{
			if (visitedSymbols[i + 1][j] == 0)
			{
				visitedSymbols[i + 1][j] = 1;
				notVisitedSymbols[i + 1][j] = ' ';

				SecondColumnWidth(visitedSymbols, notVisitedSymbols, i, j, isLeftDependent, isRightDependent, prevChoice);

				i++;

				SecondColumnWidth(visitedSymbols, notVisitedSymbols, i, j, isLeftDependent, isRightDependent, prevChoice);
			}
			else
				continue;
		}
		prevChoice = choice;
	}


	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			cout << notVisitedSymbols[i][j];
		}
		cout << endl;
	}
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
