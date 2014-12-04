#include "Security.h"

char Security::passwordFile[] = "plist.txt";

bool Security::checkUsernameAndPassword(const char *username, const char *password){
	char puser[10];
	char ppass[10];
	char file_data[20];
	ifstream infile;
	infile.open(Security::passwordFile);
	infile >> file_data;
	infile.close();
	sscanf(file_data, "%[^/]/%[^/]", puser, ppass);
	if( strcmp(puser, username) == 0 && strcmp(ppass, password) == 0 ){
		return true;
	}else{
		return false;
	}
}