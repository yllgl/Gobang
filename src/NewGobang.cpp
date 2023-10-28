#include <iostream>
#include"Controller.h"
#include <stdlib.h>
int main()
{
	system("chcp 65001 & cls");
	Controller control = Controller();
	control.Game();
	return 0;
}
