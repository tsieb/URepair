#pragma once
#include <string>

namespace job_db
{

struct Job{

enum{
DELETED_ISSUE = -1
};

string title;
int issue{DELETED_ISSUE};
string desc;
string location;
int status;
int price;
int time;


};
  
string toJSON(const Job& job);  
Job fromJSON(const string&json);
}

namespace user_db
{

struct User{

enum{
DELETED_ISSUE = -1
};

string username;
string name;
int issue{DELETED_ISSUE};
string address;
string email;
string phone;
string type;

};
  
string toJSON(const User& user);  
User fromJSON(const string&json);
}
