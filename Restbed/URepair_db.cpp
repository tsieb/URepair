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

        void log_if(bool expression, const Level level, const char* format,
            ...) override
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
  
JobDb load()
    {
        JobDb db;
        db.emplace_back(fromJson(
            R"json({"title":"","desc":"","location":"","status":1,"price":150,"time":})json"));
        {
            Comic comic;
            comic.title = "The Fantastic Four";
            comic.desc = "";
            comic.location = "";
            comic.status = 1;
            comic.price = 150;
            comic.time = ;
            db.push_back(job);
        }
        return db;
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
   if (job.title.empty() || job.desc.empty() || job.location.empty() || job.status < 1 || job.status > 3 || job.price < 0 || job.time < 0 || job.time > 12 //or 24)
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
                 
void viewJob(const SessionPtr& session, ComicDb& db){
  std::size_t id{};
  if (validId(session, db, id))
  {
    const std::string json = toJson(db[id]);
    session->close(restbed::OK, json, { {"Content-Type", "application/json"}, {"Content-Length", std::to_string(json.size())} });
  }
}

void deleteJob(const SessionPtr& session, ComicDb& db){
  std::size_t id{};
  if (validId(session, db, id))
  {
    {
      std::unique_lock<std::mutex> lock(g_dbMutex);
      db[id] = Comic{};
    }
    session->close(restbed::OK);
  }
}

  
void addUser(){
  
}

void editUser(){
  
}

void deleteUser(){
  
}

void runService()
    {
        JobDb db = load();

        restbed::Service job_service;
        publishResources(service, db);
        job_service.set_logger(std::make_shared<CustomLogger>());
        job_service.start(getSettings());
    }                 
                 
}//namespace URepairdb

int main(){
  try
    {
        jobdb::runService();
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
