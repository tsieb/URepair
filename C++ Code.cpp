#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <"User.h">
#include <"Job.h">
using namespace std;

class User {
	User(){

	};

	~User(){

	};

private:
	string username;
	string type;
	string phone;
	string address;
	string email;
	string name;
	
public:
	string getName();
	string getUsername();
	string getType();
	string getPhone();
	string getEmail();
	string getAddress();

	string setName();
	string setUsername();
	string setType();
	string setPhone();
	string setEmail();
	string setAddress();

	string setName(string n) {
		name = n;
	}
	string getUsername(string u){
		username = u;
	}
	string setType(string t) {
		type = t; 
	}

	string setPhone(string p) {
		phone = p; //Alternative variable may be needed
	}

	string setEmail(string e) {
		email = e;
	}

	string setAddress(string a) {
		address = a;
	}
	
	bool Type();
	string login();

};

string User::getName() {
	return name;
}
string getUsername(){
	return username;
}
string User::getType() {
	return type;
}
string User::getPhone() {
	return phone;
}
string User::getEmail() {
	return email;
}
string User::getAddress() {
	return address;
}

bool Type() {
	getType();
	if (type == "customer") {
		return true;
	}
	else {
		return false;
	}
}

class Job{

	Job() {

	};

	~Job() {

	};

private:
	int time;
	int status;
	int price;
	string desc;
	string location;
	string title;

public:
	int getTime(); //Grab from computer clock
	int getPrice();
	int getStatus();
	string getLocation();
	string getDesc();
	string getTitle();

	int setTime();
	int setPrice();
	int setStatus();
	string setLocation();
	string setDesc();
	string setTitle();

	int setTime(int t) {
		time = t;
	}
	string setLocation(string l) {
		location = l;
	}
	int setPrice(int p) {
		price = p;
	}
	int setStatus(int s) {
		status = s;
	}
	string setDesc(string d) {
		desc = d;
	}
	string setTitle(string t) {
		title = t; //Alternative variable may be needed
	}
};
	
int Job::getTime() {
	return time;
}
string Job::getLocation() {
	return location;
}
int Job::getPrice() {
	return price;
}
int Job::getStatus() {
	return status;
}
string Job::getDesc() {
	return desc;
}
string Job::getTitle() {
	return title;
}


struct Node {
	int price;
	string price;
	struct Node* next;
};

template<typename Type>
void data_storage(Type data) {
	//temp array to store current data w/ .txt as backup
	//array[6]={data};
}

int Filter() {

	int filter;

	if (filter = 1) {
		//date
		string line;
		ifstream joblist;
		while (!joblist.eof()) {
			cout << line << '\n';
		}
		if (joblist.close()) {
			//Go to account dashboard
		}
	}
	if (filter = 2) {
		//price
		ifstream joblist;
		joblist.open("Jobs.txt");
		
		while (!joblist.eof()) {
			
			

		}
		if (joblist.close()) {
			//Go to account dashboard
		}
	}
	if (filter = 3) {
		//type
		ifstream joblist;
		joblist.open("Jobs.txt");
		
		
		while (!joblist.eof()) {
			
		}

		if (joblist.close()) {
			//Go to account dashboard
		}
	}
}

void ViewJob() {

	ifstream joblist;

	string line;
	joblist.open("Jobs.txt");
	while (getline(joblist, line)) {
			cout << line << '\n';
		}

		if (joblist.close()) {
			//Go to account dashboard
		}
}

void AddJob() {

	ofstream joblist;
	
	if (joblist.open()) {
		joblist << " \n";
		joblist.close();
	}

	else {
		cout << "Error";
		return 0; //To account dashboard
	}

}

int main() {

	//Insert file info

	User user;	

	Job job;

	return 0;
}
