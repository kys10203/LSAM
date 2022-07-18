#include "jigapp.h"

int main(int argc, char ** argv)
{
	int ret = 0;

	cout << endl << endl << endl;
	cout << "******************************" << endl;
	cout << "***    START JIG_APP!!!    ***" << endl;
	cout << "******************************" << endl << endl;

	CJigApp tApp;

	if(tApp.Run() == false)
	{
		ret = -1;
	}
	else
	{
		ret = 0;
	}

	return ret;
}

