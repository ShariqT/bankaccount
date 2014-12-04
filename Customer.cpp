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

void Customer::searchByAccountNumber(int account_number){
	ifstream customer_database;
	customer_database.open(this->data_file, ios::binary | ios::in);

	ifstream metainfo;	
	metainfo.open(this->header_file, ios::in);

	int read_account_number;
	int byte_order;
	while(metainfo >> read_account_number >> byte_order ){
		if(read_account_number == account_number){
			//found the record we were searching for!!!
			//move to the first set of the byte and then move up four to get to the 
			//name attribute
			int size_of_string;
			stringstream ss;
			customer_database.seekg(byte_order + 4, ios::beg);
			this->setAccountNumber(account_number);
			customer_database.read((char *)&size_of_string, 8);
			for(int x = 0; x < size_of_string; x++){
				char c;
				customer_database.read((char *)&c, sizeof(char));
				ss << c;
			}
			
			customer_database.read((char *)(&saving), sizeof(double));
			customer_database.read((char *)(&checking), sizeof(double));
			customer_database.read((char *)(&has_saving), sizeof(bool));
			customer_database.read((char *)(&has_checking), sizeof(bool));
			name = ss.str();
			this->setAccessStatus(true);
		}
	}

	if(customer_database.fail()){
		perror("Could not read everything from file");
	}

	customer_database.close();
	metainfo.close();
	return;

}



void Customer::setAccountNumber(int account_number){
	this->account_number = account_number;
}



void Customer::create(){
	cout << "Saving customer information..." << endl;

	fstream customer_database;
	customer_database.open(this->data_file, ios::binary | ios::out | ios::in);
	
	fstream metainfo;
	metainfo.open(this->header_file, ios::out | ios::in);

	int last_record;
	int byte_order;
	//determine what the customer ID should be by going to through the file and finding the last
	//id that was inserted. 

	//but first let's see if the file is empty or not
	metainfo.seekg(0, ios::end);
	size_t len = metainfo.tellg();
	if(len == 0){
		//file is empty, so let's start at the beginning. account id will be 1001.
		this->account_number = 1001;
		byte_order = 0;
	}else{
		//move the last record and get the account number for it and increase it by one.
		//get last byte position records and increase by one as well
		metainfo.seekg(0, ios::beg);
		while(metainfo >> last_record >> byte_order){}
		this->account_number = ++last_record;
		++byte_order;
		
		metainfo << last_record << endl;
		metainfo << byte_order << endl;
	}
customer_database.seekg(byte_order, ios::beg);
	
	size_t strlength = name.size();
	//create the customer record in the file: byte order is account_number, length of name, name, saving, checking, has_saving, has_checking
	customer_database.write((char *)(&account_number), sizeof(account_number));
	customer_database.write((char *)(&strlength), sizeof(name.size()));
	customer_database.write(name.c_str(), strlength);
	customer_database.write((char *)(&saving), sizeof(saving));
	customer_database.write((char *)(&checking), sizeof(checking));
	customer_database.write((char *)(&has_saving), sizeof(has_saving));
	customer_database.write((char *)(&has_checking), sizeof(has_saving));
	cout << customer_database.gcount() << endl;
	if(customer_database.fail()){
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

		exit(EXIT_FAILURE);
	}

	metainfo << account_number << endl;
	metainfo << byte_order << endl;
	customer_database.close();
	metainfo.close();
}