#include <iostream>
#include <string>
#include <cstdlib>
#include "Security.h"
#include "Customer.h"
#include "Menus.h"
#include <string.h>
#include <vector>

using namespace std;

static string username;
static string password;
int static num_of_times_log_in_tried = 1;

bool welcomeScreen(){
	system("clear");
	cout << "Welcome to the Example Bank Trust." << endl;
	cout << "Please login." << endl;
	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	cin >> password;
	return Security::checkUsernameAndPassword(username.c_str(), password.c_str());
}


void badCredentials(){
	system("clear");
	cout << "Wrong user and password combination." << endl;
}


void findByName(){
	system("clear");
	cout << "Finding Customer By Name";
	string search_str;
	cin.ignore();
	cin >> search_str;
	/*vector<Customer> search_results;
	Customer::searchByName(&search_results, search_str);
	cout << search_results.size();
	for(search_results<Customer>::iterator i = values.begin(); i != values.end(); i++){

	}*/
}

void findByAccount(){
	system("clear");
	int account_number;
	string selection;
	Customer *found_customer;
	cout << "Finding Customer By Account Number" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "Enter Customer Account Number:";
	cin.ignore();
	cin >> account_number;
	//Customer has a static function that returns a pointer to a Customer object.
	//if the Customer->ACCESS_STATUS attribute is false, then there is no customer that matches 
	//the account number. 
	found_customer = Customer::searchByAccountNumber(account_number);
	if(found_customer != 0){
		editCustomerInformation(found_customer);
	}else{
		cout << "***Could not find a customer with that account number***" << endl;
		cout << "Would you like to enter the account number again?" << endl;
		cin.ignore();
		cin >> selection;
		if(selection.compare("y") == 0){
			findByAccount();
		}else{
			showMainMenu();
		}
	}
}


void editCustomerInformation(Customer *customer){
	system("clear");
	cout << "Name :" << customer->name << endl;
	cout << "Account Number: " << customer->getAccountNumber() << endl;

}

void addCustomer(){
	system("clear");
	Customer customer;
	int account_choice;
	cout << "Adding a New Customer" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "** Customer Service Note: Make new customer has two forms of ID **" << endl;
	cout << "Enter Customer Name:";
	cin.ignore();
	getline(cin, customer.name);
	
	cout << "What type of account does the customer wish to open: " << endl;
	cout << "1) Saving" << endl;
	cout << "2) Checking" << endl;
	cout << "3) Both" << endl;
	cin >> account_choice;
	switch(account_choice){
		case 1:
			customer.has_saving = true;
			addMoney("saving", &customer);
		break;
		case 2:
			customer.has_checking = true;
			addMoney("checking", &customer);
		break;
		case 3:
			customer.has_checking = true;
			customer.has_saving = true;
			addMoney("both", &customer);
		break;
	}

	
	customer.create();
	
	//showMainMenu();


}

void addMoney(string type, Customer *customer){
	string yes_or_no_deposit;
	string any_money_message = "Does the customer money to deposit? (Enter 'y' for Yes and 'n' for No)";
	string starting_account_message = "Enter starting account amount:";
	
	cout << any_money_message << endl;
	cin.ignore();
	cin >> yes_or_no_deposit;

	if(yes_or_no_deposit.compare("y") == 0){
		if(type.compare("saving") == 0){
			cout << starting_account_message << endl;
			cin >> customer->saving;
			return;
		}else if(type.compare("checking") == 0){
			cout << starting_account_message << endl;
			cin >> customer->checking;
			return;
		}else{
			cout << starting_account_message << " For Checking" << endl;
			cin >> customer->checking;
			cout << starting_account_message << " For Saving" << endl;
			cin >> customer->saving;
			return;
		}
	}else{
		return;
	}
	
}

void showMainMenu(){
	system("clear");
	int selection = 0;
	cout << "Example Bank Trust Main Menu" << endl;
	cout << "1) Find Customer By Name" << endl;
	cout << "2) Find Customer By Account Number" << endl;
	cout << "3) Add New Customer" << endl;
	cin.ignore();
	cin >> selection;
	//check to see if this is an actual integer
		switch(selection){
			case 1: 
				findByName();
			break;
			case 2:
				findByAccount();
			break;
			case 3:
				addCustomer();
			break;
			default:
				cout << "Not a valid selection; shutting down!";
			break;
		}
	return;
}

int main(){

	for(;;){
		if( welcomeScreen() ){
			showMainMenu();
			break;
		}else{
			badCredentials();
			if(num_of_times_log_in_tried != 3){
				++num_of_times_log_in_tried;
				continue;
			}else{
				system("clear");
				cout << "Three (3) failed login attempts;system shutting down." << endl;
				cout << "Please contact the system administrator for help." << endl;
				return 1;
			}
		}
	}

	
}

