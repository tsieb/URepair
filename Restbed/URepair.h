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
