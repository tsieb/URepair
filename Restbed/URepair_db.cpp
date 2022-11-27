#include <iostream>
#include <restbed>
#include <string>
#include <"Job.h">
#include <"User.h">
#include <vector>
#include <mutex>

namespace URepairdb{
  
}

bool validId(const SessionPtr &session, const JobDb &db, std::size_t &id)
{
    const auto &request = session->get_request();
    if (request->has_path_parameter("id"))
    {
        id = request->get_path_parameter("id", 0);
        if (id < db.size() && db[id].issue != Job::DELETED_ISSUE)
        {
            return true;
        }
        notAcceptable(session, "Not Acceptable, id out of range");
    }
    else
    {
        notAcceptable(session, "Not Acceptable, missing id");
    }
    return false;
}

void addUser(){
  
}

void editUser(){
  
}

void deleteUser(){
  
}

void addJob(){
  
}

void viewJob(){
  
}

void deleteJob(){
  
}
  


int main(){
  
}
