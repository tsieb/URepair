#ifndef USER_H
#define USER_H

class User {

private:
  string name;
	string username;
	string type;
	string phone;
	string address;
	string email;
	
public:
	string getName(){ return name; }
	string getUsername(){ return username; }
	string getType(){ return type; }
	string getPhone(){ return phone; }
	string getEmail(){ return email; }
	string getAddress(){ return address; }
}
#endif
