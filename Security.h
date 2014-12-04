#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

class Security{
	public:
		static char passwordFile[];
		static bool checkUsernameAndPassword(const char *username, const char *password);
};