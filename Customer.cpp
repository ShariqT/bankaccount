#include "Customer.h"

char Customer::data_file[] = "customers.dat";
char Customer::header_file[] = "metainfo.dat";

Customer::Customer(){
	
	this->saving = 0.00;
	this->checking = 0.00;
	this->has_saving = false;
	this->has_checking = false;
	this->STATUS = false;
	this->account_number = 0;
}


int Customer::getAccountNumber(){
	return this->account_number;
}

void Customer::setAccessStatus(bool status){
	this->STATUS = status;
	return;
}

bool Customer::getAccessStatus(){
	return this->STATUS;
}


void Customer::searchByName(vector<Customer *> *search_results, string search_str){
	ifstream customer_database;
	ifstream metainfo;

	customer_database.open(Customer::data_file, ios::binary | ios::in);
	metainfo.open(Customer::header_file, ios::in);
	int read_account_number;
	int byte_order;
	int size_of_string;
	while(metainfo >> read_account_number >> byte_order){
		stringstream ss;
		customer_database.seekg(byte_order + 4, ios::beg);
		customer_database.read((char *)&size_of_string, 8);
		for(int x = 0; x < size_of_string; x++){
			char c;
			customer_database.read((char *)&c, sizeof(char));
			ss << c;
		}
		string to_be_search = ss.str();

		//check to see if it matches the name
		if(to_be_search.find(search_str) != string::npos){
			cout << "I found something!" << endl;
			search_results->push_back(new Customer());

			customer_database.read((char *)(&search_results->back()->saving), sizeof(double));
			customer_database.read((char *)(&search_results->back()->checking), sizeof(double));
			customer_database.read((char *) (&search_results->back()->has_saving), sizeof(bool));
			customer_database.read((char *)(&search_results->back()->has_checking), sizeof(bool));
			search_results->back()->setAccessStatus(true);
			search_results->back()->setAccountNumber(read_account_number);
			search_results->back()->name = ss.str();			
		}
	}

}

Customer* Customer::searchByAccountNumber(int account_number){
	ifstream customer_database;
	customer_database.open(Customer::data_file, ios::binary | ios::in);

	ifstream metainfo;	
	metainfo.open(Customer::header_file, ios::in);

	int read_account_number;
	int byte_order;
	Customer *return_address = 0;
	while(metainfo >> read_account_number >> byte_order ){
		if(read_account_number == account_number){
			Customer *found_customer = new Customer();
			//found the record we were searching for!!!
			//move to the first set of the byte and then move up four to get to the 
			//name attribute
			int size_of_string;
			stringstream ss;
			customer_database.seekg(byte_order + 4, ios::beg);
			found_customer->setAccountNumber(account_number);
			customer_database.read((char *)&size_of_string, 8);
			for(int x = 0; x < size_of_string; x++){
				char c;
				customer_database.read((char *)&c, sizeof(char));
				ss << c;
			}
			
			customer_database.read((char *)(&found_customer->saving), sizeof(double));
			customer_database.read((char *)(&found_customer->checking), sizeof(double));
			customer_database.read((char *)(&found_customer->has_saving), sizeof(bool));
			customer_database.read((char *)(&found_customer->has_checking), sizeof(bool));
			found_customer->name = ss.str();
			found_customer->setAccessStatus(true);
			return_address = found_customer;
		}
	}

	if(customer_database.fail()){
		perror("Could not read everything from file");
	}

	customer_database.close();
	metainfo.close();
	return return_address;

}



void Customer::setAccountNumber(int account_number){
	this->account_number = account_number;
}

void Customer::save(){
	cout << "Saving updated customer info..." << endl;
	fstream customer_database;
	ifstream metainfo_read;
	ofstream metainfo_output;
	int num;
	int sel_num;
	int sel_byte;
	int byte_order;
	vector<int> metainfo_contents; 
	customer_database.open(Customer::data_file, ios::binary| ios::in| ios::out);
	metainfo_read.open(Customer::header_file);
	while(metainfo_read >> num >> byte_order){
		if(this->account_number == num){
			sel_num = num;
			sel_byte = byte_order;
		}
		metainfo_contents.push_back(num);
		metainfo_contents.push_back(byte_order);
	}
	metainfo_read.close();
	customer_database.seekg(0, ios::end);
	int new_byte = customer_database.tellg();
	this->_write(&customer_database);
	metainfo_output.open(Customer::header_file, ios::trunc);
	for(unsigned int i = 0; i < metainfo_contents.size(); i++){
		if(metainfo_contents.at(i) == sel_num){
			metainfo_output << sel_num << endl;
			continue;
		}

		if(metainfo_contents.at(i) == sel_byte){
			metainfo_output << new_byte << endl;
			continue;
		}

		metainfo_output << metainfo_contents.at(i) << endl;
	}

	metainfo_output.close();
	customer_database.close();
}


void Customer::_write(fstream *file){
	file->seekg(0, ios::end);
	
	size_t strlength = name.size();
	//create the customer record in the file: byte order is account_number, length of name, name, saving, checking, has_saving, has_checking
	file->write((char *)(&account_number), sizeof(account_number));
	file->write((char *)(&strlength), sizeof(name.size()));
	file->write(name.c_str(), strlength);
	file->write((char *)(&saving), sizeof(saving));
	file->write((char *)(&checking), sizeof(checking));
	file->write((char *)(&has_saving), sizeof(has_saving));
	file->write((char *)(&has_checking), sizeof(has_saving));
	
	if(file->fail()){
		switch(errno){
			case EACCES:
				cout << "Could not write to the file. Either the drive was not ready or permission was denied" << endl;
				break;
			case ENOENT:
				cout << "Could not find this file" << endl;
			break;
			default:
				perror("Opening data file");
		}
	}

}


void Customer::create(){
	cout << "Saving customer information..." << endl;

	fstream customer_database;
	
	customer_database.open(Customer::data_file, ios::binary | ios::out | ios::in);
	
	fstream metainfo;
	metainfo.open(Customer::header_file, ios::out | ios::in | ios::app);

	int last_record;
	int byte_order = 0;
	//determine what the customer ID should be by going to through the file and finding the last
	//id that was inserted. 

	//but first let's see if the file is empty or not
	metainfo.seekg(0, ios::end);
	size_t len = metainfo.tellg();
	metainfo.seekg(0, ios::beg);
	if(len == 0){
		//file is empty, so let's start at the beginning. account id will be 1001.
		this->account_number = 1001;
	}else{
		//move the last record and get the account number for it and increase it by one.
		
		while(metainfo >> last_record >> byte_order){
			cout << "last record is " << last_record << endl;
			cout << "byter order is " << byte_order << endl;
			this->account_number = ++last_record;
		}
		customer_database.seekg(0, ios::end);
		byte_order = customer_database.tellg();		
	}
	cout << this->account_number << endl;
		cout << byte_order << endl;
		metainfo.clear();
	
	this->_write(&customer_database);	
	
	if(metainfo.fail()){
		switch(errno){
			case EACCES:
				cout << "Could not write to the file. Either the drive was not ready or permission was denied" << endl;
				break;
			case ENOENT:
				cout << "Could not find this file" << endl;
			break;
			default:
				cout << strerror(errno) << endl;
		}

		
	}

	metainfo.seekp(0, ios::end);
	metainfo << this->account_number << endl;
	metainfo << byte_order << endl;
	metainfo.close();
	customer_database.close();
	
}