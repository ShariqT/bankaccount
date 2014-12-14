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
	string selection;
	unsigned int search_sel;
	cout << "Finding Customer By Name" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "Enter Customer's Name (case-senistive):" << endl;
	string search_str;
	cin.ignore();
	cin >> search_str;
	vector<Customer *> *search_results = new vector<Customer *>();
	Customer::searchByName(search_results, search_str);
	
	if(search_results->size() != 0){
		for(size_t i = 0; i < search_results->size(); i++){
			cout << "Search Results: " << endl;
			cout << i+1 << ") " << search_results->at(i)->name << endl;
		}
		cin.ignore();
		cout << "Select the number of the account you would like to edit: " << endl;
		cin >> search_sel;
		if(search_sel > search_results->size()){
			cout << "That was not an available option. Please select again." << endl;
			cin.ignore();
			cin >> search_sel;
		}else{
			editCustomerInformation(search_results->at(0));
		
			//clean up the memory
			for(size_t i = 0; i < search_results->size(); i++){
				delete search_results->at(i);
			}
			search_results->clear();
			delete search_results;
		}

		
		
	}else{

		cout << "Could not find any customer's with that name!" << endl;
		cout << "Would you like to try again? (y for Yes and n for No)" << endl;
		cin.ignore();
		cin >> selection;
		if(selection.compare("y") == 0){
			findByName();
		}else{
			showMainMenu();
		}
	}
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
	string selection;
	double amt;
	cout << "Name: " << customer->name << endl;
	cout << "Account Number: " << customer->getAccountNumber() << endl;

	if(customer->has_saving){
		cout << "Savings: " << customer->saving << endl;
	}

	if(customer->has_checking){
		cout << "Checking: " << customer->checking << endl;
	}

	cout << "---------------------------------" << endl;


	editMenu();

	

	while (selection.compare("save") != 0){
		cout << "Enter selection--" << endl;
		
		cin >> selection;

		if(selection.compare("name") == 0){
			cin.ignore();
			cout << "Enter the new name: ";
			getline(cin, customer->name);
			cout << "**customer name changed to " << customer->name << "**" << endl;

		}

		if(selection.compare("saving") == 0){
			
			cout << "Enter new saving amout: ";
			cin.ignore();
			cin >> amt;
			cin.ignore();
			customer->saving = amt;
			customer->has_saving = true;
			cout << "**customer saving changed to " << customer->saving << "**" << endl;
		}

		if(selection.compare("checking") == 0){
			
			cout << "Enter new checking amount:";
			cin.ignore();
			cin >> amt;
			cin.ignore();
			customer->checking = amt;
			customer->has_checking = true;
			cout << "customer checking changed to " << customer->checking << "**" << endl;
		}

	}

	customer->save();

}


void editMenu(){
	cout << "What would you like to edit? Enter the word within parentheses." << endl;
	cout << "(name) Account Holder's Name" << endl;
	cout << "(saving) Saving Account" << endl;
	cout << "(checking) Checking Account" << endl;
	cout << "(save) Save Edited Information" << endl;
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

