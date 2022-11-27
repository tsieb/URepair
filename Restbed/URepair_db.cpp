#include <iostream>
#include <restbed>
#include <string>
#include <"Job.h">
#include <"User.h">
#include <vector>
#include <mutex>

namespace URepairdb{

std::mutex g_dbMutex;
using JobDb = std::vector<Job>;
using UserDb = std::vector<User>;
using SessionPtr = std::shared_ptr<restbed::Session>;  

class CustomLogger : public restbed::Logger
    {
    public:
        void stop() override {}

        void start(const std::shared_ptr<const restbed::Settings>&) override {}

        void log(const Level, const char* format, ...) override
        {
            std::va_list arguments;
            va_start(arguments, format);

            vfprintf(stderr, format, arguments);
            fprintf(stderr, "\n");

            va_end(arguments);
        }

        void log_if(bool expression, const Level level, const char* format, ...) override
        {
            if (expression)
            {
                std::va_list arguments;
                va_start(arguments, format);
                log(level, format, arguments);
                va_end(arguments);
            }
        }
    };
  
JobDb job_load()
    {
        JobDb db;
        db.emplace_back(fromJson(
            R"json({"title":"","desc":"","location":"","status":1,"price":150,"time":0})json"));
        {
            Job job;
            job.title = "The Fantastic Four";
            job.desc = "";
            job.location = "";
            job.status = 1;
            job.price = 150;
            job.time = 0;
            db.push_back(job);
        }
        return db;
    }  
    
UserDb use_load()
    {
        UserDb u_db;
        db.emplace_back(fromJson(R"json({"username":"John_Doe1234","name":"John Doe","address":"1234 Seymour Street, Kamloops, BC, A1C 2C3","email":john_doe@urepair.ca,"phone":2503161234,"type":"Customer"})json"));
        {
            Job job;
            job.username = "John_Doe1234";
            job.name = "John Doe";
            job.address = "1234 Seymour Street, Kamloops, BC, A1C 2C3";
            job.email = "john_doe@urepair.ca";
            job.phone = "2503161234";
            job.type = "Customer";
            db.push_back(job);
        }
        return u_db;
    }   
    
std::shared_ptr<restbed::Settings> getSettings()
{
  auto settings = std::make_shared<restbed::Settings>();
  settings->set_default_header("Connection", "close");
  return settings;
}  
  
void notAcceptable(const SessionPtr& session, const std::string& msg)
{
  session->close(restbed::NOT_ACCEPTABLE, msg,
  { {"Content-Type", "text/plain"},
  {"Content-Length", std::to_string(msg.size())} });
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

void addJob(const SessionPtr& session, ComicDb& db){
  auto& request = session->get_request();
  std::size_t length{};
  length = request->get_header("Content-Length", length);
  if (length == 0)
  {
    notAcceptable(session, "Not Acceptable, empty body");
    return;
  }

  session->fetch(length, [&db](const SessionPtr& session, const restbed::Bytes& data)
   {
     const std::string json{ reinterpret_cast<const char*>(data.data()), data.size() 
   };
   Job job = fromJson(json);
   if (job.title.empty() || job.desc.empty() || job.location.empty() || job.status < 1 || job.status > 3 || job.price < 0 || job.time < 0 || job.time > 12 /*or 24*/)
        {
            notAcceptable(session, "Not Acceptable, invalid JSON");
            return;
        }

        {
            std::unique_lock<std::mutex> lock(g_dbMutex);
            db.push_back(job);
        }
        session->close(restbed::OK);
    });
}

void publishJob(restbed::Service& service, JobDb& db)
    {
        auto jobResource = std::make_shared<restbed::Resource>();
        jobResource->set_path("/comic/{id: [[:digit:]]+}");
        jobResource->set_method_handler("GET", [&db](const SessionPtr& session)
            { return viewJob(session, db); });
        jobResource->set_method_handler("DELETE", [&db](const SessionPtr& session)
            { return deleteJob(session, db); });
        jobResource->set_method_handler("PUT", [&db](const SessionPtr& session)
            { return updateJob(session, db); });
        service.publish(jobResource);

        auto createJobResource = std::make_shared<restbed::Resource>();
        createJobResource->set_path("/comic");
        auto createJobCallback = [&db](const SessionPtr& session)
        { return createJob(session, db); };
        createJobCallback->set_method_handler("PUT", createJobCallback);
        createJobCallback->set_method_handler("POST", createJobCallback);
        service.publish(createJobCallback);
    }                 

void updateJob(const SessionPtr& session, JobDb& db)
    {
        std::size_t id{};
        if (!validId(session, db, id))
            return;

        auto& request = session->get_request();
        std::size_t length{};
        length = request->get_header("Content-Length", length);
        if (length == 0)
        {
            notAcceptable(session, "Not Acceptable, empty request body");
            return;
        }

        session->fetch(
            length,
            [&db, id](const SessionPtr& session, const restbed::Bytes& data)
            {
                const std::string json{ reinterpret_cast<const char*>(data.data()), data.size() 
            };
        Job job = fromJson(json);
        if (job.title.empty() || job.desc.empty() || job.location.empty() || job.status < 1 || job.status > 3 || job.price < 0 || job.time < 0 || job.time > 12 /*or 24*/)
        {
            notAcceptable(session, "Not Acceptable, invalid JSON");
            return;
        }

        {
            std::unique_lock<std::mutex> lock(g_dbMutex);
            db[id] = job;
        }
        session->close(restbed::OK);
            });
    }                 

void viewJob(const SessionPtr& session, JobDb& db){
  std::size_t id{};
  if (validId(session, db, id))
  {
    const std::string json = toJson(db[id]);
    session->close(restbed::OK, json, { {"Content-Type", "application/json"}, {"Content-Length", std::to_string(json.size())} });
  }
}

void deleteJob(const SessionPtr& session, JobDb& db){
  std::size_t id{};
  if (validId(session, db, id))
  {
    {
      std::unique_lock<std::mutex> lock(g_dbMutex);
      db[id] = Job{};
    }
    session->close(restbed::OK);
  }
} //Job Information

  
void addUser(const SessionPtr& session, UserDb& u_db){
  auto& request = session->get_request();
  std::size_t length{};
  length = request->get_header("Content-Length", length);
  if (length == 0)
  {
    notAcceptable(session, "Not Acceptable, empty body");
    return;
  }

  session->fetch(length, [&u_db](const SessionPtr& session, const restbed::Bytes& data)
   {
     const std::string json{ reinterpret_cast<const char*>(data.data()), data.size() 
   };
   User user = fromJson(json);
   if (user.username.empty() || user.name.empty() || user.address.empty() || user.email.empty()|| user.phone.empty() || user.type.empty())
        {
            notAcceptable(session, "Not Acceptable, invalid JSON");
            return;
        }

        {
            std::unique_lock<std::mutex> lock(g_dbMutex);
            u_db.push_back(user);
        }
        session->close(restbed::OK);
    });
}

void editUser(const SessionPtr& session, UserDb& u_db){
  std::size_t id{};
        if (!validId(session, u_db, id))
            return;

        auto& request = session->get_request();
        std::size_t length{};
        length = request->get_header("Content-Length", length);
        if (length == 0)
        {
            notAcceptable(session, "Not Acceptable, empty request body");
            return;
        }

        session->fetch(
            length,
            [&u_db, id](const SessionPtr& session, const restbed::Bytes& data)
            {
                const std::string json{ reinterpret_cast<const char*>(data.data()), data.size() };
        User user = fromJson(json);
        if (user.username.empty() || user.name.empty() || user.address.empty() || user.email.empty() || user.phone.empty() || user.type.empty())
        {
            notAcceptable(session, "Not Acceptable, invalid JSON");
            return;
        }

        {
            std::unique_lock<std::mutex> lock(g_dbMutex);
            u_db[id] = user;
        }
        session->close(restbed::OK);
            });
}

void deleteUser(const SessionPtr& session, UserDb& u_db){
  std::size_t id{};
  if (validId(session, u_db, id))
  {
    {
      std::unique_lock<std::mutex> lock(g_dbMutex);
      db[id] = User{};
    }
    session->close(restbed::OK);
  }
} //User Information

void runService()
    {
        JobDb db = job_load();
        UserDb u_db = use_load();
        restbed::Service job_service;
        publishResources(service, db);
        job_service.set_logger(std::make_shared<CustomLogger>());
        job_service.start(getSettings());
    }                 
                 
}//namespace URepairdb

int main(){
  try
    {
        URepairdb::runService();
    }
    catch (const std::exception& bang)
    {
        std::cerr << bang.what() << '\n';
        return 1;
    }
    catch (...)
    {
        return 1;
    }

    return 0;
}
