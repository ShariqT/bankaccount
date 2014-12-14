#include <iostream>
#include <string>
#include <fstream>
#include <cerrno>
#include <cstdlib>
#include <sstream>
#include <string.h>
#include <vector>


using namespace std;

class Customer{
	public:
		string name;
		bool has_saving;
		bool has_checking;
		double saving;
		double checking;
		
		Customer();
		void save();
		void create();
		void _write(fstream *file);
		void setAccountNumber(int account_number);
		int getAccountNumber();
		void setAccessStatus(bool status);
		bool getAccessStatus();
		static Customer* searchByAccountNumber(int account_number);
		void static searchByName(vector<Customer *> *search_results, string search_str);

	protected:
		static char data_file[];
		static char header_file[];

	private:
		int account_number;
		bool STATUS;
		
};