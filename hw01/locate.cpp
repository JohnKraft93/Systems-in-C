#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int main()
{
	string x;
	int y = 0;
	while (cin >> x) {
		if (y == 257) {
			printf("%s\n", x);
		}
		y++;
	}


}
