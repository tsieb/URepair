#include "../user.h"
#include "job.h"

#include <restbed>

#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include <sstream>

namespace urepairdb
{

std::mutex g_jobdbMutex;
std::mutex g_userdbMutex;
using jobdb = std::vector<Job>;
using userdb = std::vector<User>;
using SessionPtr = std::shared_ptr<restbed::Session>;


class CustomLogger : public restbed::Logger
{
  public:
    void stop() override {}

    void start(const std::shared_ptr<const restbed::Settings> &) override {}

    void log(const Level, const char *format, ...) override
    {
        std::va_list arguments;
        va_start(arguments, format);

        vfprintf(stderr, format, arguments);
        fprintf(stderr, "\n");

        va_end(arguments);
    }

    void log_if(bool expression, const Level level, const char *format,
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

jobdb jobLoad()
{
    jobdb db;
    db.emplace_back(jobFromJson(
        R"json({"title":"Washing Machine is Shaking","price":"750","description":"My washing machine has been making loud noises for the last few days and I'm worried that something might be broken. It only makes the sounds when I run it on a heavy duty cycle","location":"1253 5th Street, Kamloops","start_time":"1310","end_time":"1730","status":1,"userid":1,"contractorid":1})json"));
    {
        Job job;
        job.title = "Washing Machine is Rattling";
        job.price = "350";
        job.description =
            "My washing machine has been making loud noises for the last few "
            "days and I'm worried that something might be broken. It only "
            "makes the sounds when I run it on a heavy duty cycle";
        job.location = "1253 4th Street, Kamloops";
        job.start_time = "1310";
        job.end_time = "1730";
        job.status = 1;
        job.userid = 1;
        job.contractorid = 0;
        db.push_back(job);
    }
    return db;
}

userdb userLoad()
{
    userdb db;
    db.emplace_back(userFromJson(
        R"json({"name":" Jordan","email":"emailexample@gmail.com","address":"Lincoln Ave, Kamloops","phone":"1234567890","id":0})json"));
    {
        User user;
        user.name = "Trenton Sieb";
        user.email = "t00644364@mytru.ca";
        user.address = "1256 3rd Street, Kamloops";
        user.phone = "6048229346";
        user.id = 1;
        db.push_back(user);
    }
    return db;
}


std::shared_ptr<restbed::Settings> getSettings()
{
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_default_header("Connection", "close");
    settings->set_port(3000);
    return settings;
}

void notAcceptable(const SessionPtr &session, const std::string &msg)
{
    session->close(restbed::NOT_ACCEPTABLE, msg,
                   {{"Content-Type", "text/plain"},
                    {"Content-Length", std::to_string(msg.size())}});
}

bool validId(const SessionPtr &session, const jobdb &db, std::size_t &id)
{
    const auto &request = session->get_request();
    if (request->has_path_parameter("id"))
    {
        id = request->get_path_parameter("id", 0);
        if (id < db.size() && db[id].status != Job::DELETED_JOB)
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

bool validId(const SessionPtr &session, const userdb &db, std::size_t &id)
{
    const auto &request = session->get_request();
    if (request->has_path_parameter("id"))
    {
        id = request->get_path_parameter("id", 0);
        if (id < db.size() && db[id].id != User::DELETED_USER)
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

void readJob(const SessionPtr &session, const jobdb &db)
{
    std::size_t id{};
    if (validId(session, db, id))
    {
        const std::string json = toJson(db[id]);
        session->close(restbed::OK, json,
                       {{"Content-Type", "application/json"},
                        {"Content-Length", std::to_string(json.size())}});
    }
}

void readUser(const SessionPtr &session, const userdb &db)
{
    std::size_t id{};
    if (validId(session, db, id))
    {
        const std::string json = toJson(db[id]);
        session->close(restbed::OK, json,
                       {{"Content-Type", "application/json"},
                        {"Content-Length", std::to_string(json.size())}});
    }
}


void readUserJobs(const SessionPtr &session, const jobdb &db)
{
    std::size_t id{};
    if (validId(session, db, id))
    {
        const auto &request = session->get_request();
        int id = request->get_path_parameter("id", 0);
        std::stringstream ss;
        ss.clear();
        ss << "[";
        for (auto iter = db.begin(); iter != db.end(); iter++)
        {
            if (iter->userid == id && iter->status != Job::DELETED_JOB)
            {
                if (ss.str() != "[")
                {
                    ss << ",";
                }
                ss << "{";
                ss << "\"id\":\"" + std::to_string(iter - db.begin()) + "\",";
                ss << "\"title\":\"" + iter->title + "\",";
                ss << "\"price\":\"" + iter->price + "\",";
                ss << "\"description\":\"" + iter->description + "\",";
                ss << "\"location\":\"" + iter->location + "\",";
                ss << "\"start_time\":\"" + iter->start_time + "\",";
                ss << "\"end_time\":\"" + iter->end_time + "\",";
                ss << "\"userid\":" + std::to_string(iter->userid) + ",";
                ss << "\"contractorid\":" + std::to_string(iter->contractorid) +
                          ",";
                ss << "\"status:\"" + std::to_string(iter->status);
                ss << "}";
            }
        }
        ss << "]";
        const std::string json = ss.str();
        session->close(restbed::OK, json,
                       {{"Content-Type", "application/json"},
                        {"Content-Length", std::to_string(json.size())}});
    }
}

void readFilteredJobs(const SessionPtr &session, const jobdb &db)
{
    std::size_t id{};
    if (validId(session, db, id))
    {
        const auto &request = session->get_request();
        std::stringstream ss;
        int minval = request->get_path_parameter("min", 0);
        ss.clear();
        ss << "[";
        for (auto iter = db.begin(); iter != db.end(); iter++)
        {
            if (std::stoi(iter->price) >= minval && iter->status != Job::DELETED_JOB)
            {
                if (ss.str() != "[")
                {
                    ss << ",";
                }
                ss << "{";
                ss << "\"id\":\"" + std::to_string(iter - db.begin()) + "\",";
                ss << "\"title\":\"" + iter->title + "\",";
                ss << "\"price\":\"" + iter->price + "\",";
                ss << "\"description\":\"" + iter->description + "\",";
                ss << "\"location\":\"" + iter->location + "\",";
                ss << "\"start_time\":\"" + iter->start_time + "\",";
                ss << "\"end_time\":\"" + iter->end_time + "\",";
                ss << "\"userid\":" + std::to_string(iter->userid) + ",";
                ss << "\"contractorid\":" + std::to_string(iter->contractorid) + ",";
                ss << "\"status\":" + std::to_string(iter->status);
                ss << "}";
            }
        }
        ss << "]";
        const std::string json = ss.str();
        session->close(restbed::OK, json,
                       {{"Content-Type", "application/json"},
                        {"Content-Length", std::to_string(json.size())}});
    }
}

void readAcceptedJobs(const SessionPtr &session, const jobdb &db)
{
    std::size_t id{};
    if (validId(session, db, id))
    {
        const auto &request = session->get_request();
        std::stringstream ss;
        int id = request->get_path_parameter("id", 0);
        ss.clear();
        ss << "[";
        for (auto iter = db.begin(); iter != db.end(); iter++)
        {
            if (iter->contractorid == id &&
                iter->status != Job::DELETED_JOB)
            {
                if (ss.str() != "[")
                {
                    ss << ",";
                }
                ss << "{";
                ss << "\"id\":\"" + std::to_string(iter - db.begin()) + "\",";
                ss << "\"title\":\"" + iter->title + "\",";
                ss << "\"price\":\"" + iter->price + "\",";
                ss << "\"description\":\"" + iter->description + "\",";
                ss << "\"location\":\"" + iter->location + "\",";
                ss << "\"start_time\":\"" + iter->start_time + "\",";
                ss << "\"end_time\":\"" + iter->end_time + "\",";
                ss << "\"userid\":" + std::to_string(iter->userid) + ",";
                ss << "\"contractorid\":" + std::to_string(iter->contractorid) +
                          ",";
                ss << "\"status\":" + std::to_string(iter->status);
                ss << "}";
            }
        }
        ss << "]";
        const std::string json = ss.str();
        session->close(restbed::OK, json,
                       {{"Content-Type", "application/json"},
                        {"Content-Length", std::to_string(json.size())}});
    }
}

void updateJob(const SessionPtr &session, jobdb &db)
{
    std::size_t id{};
    if (!validId(session, db, id))
        return;

    auto &request = session->get_request();
    std::size_t length{};
    length = request->get_header("Content-Length", length);
    if (length == 0)
    {
        notAcceptable(session, "Not Acceptable, empty request body");
        return;
    }
    
    session->fetch(
        length,
        [&db, id](const SessionPtr &session, const restbed::Bytes &data)
        {
            const std::string json{reinterpret_cast<const char *>(data.data()),
                                   data.size()};
            
            Job job = jobFromJson(json);
            
            if (job.title.empty() || job.status < 1 || job.price.empty() ||
                job.description.empty() || job.location.empty() ||
                job.start_time.empty() || job.end_time.empty() ||
                job.userid > 0)
            {
                notAcceptable(session, "Not Acceptable, invalid JSON");
                return;
            }

            {
                std::unique_lock<std::mutex> lock(g_jobdbMutex);
                db[id] = job;
            }
            session->close(restbed::OK);
        });
}

// Not Currently implemented
void updateUser(const SessionPtr &session, userdb &db)
{
    std::size_t id{};
    if (!validId(session, db, id))
        return;

    auto &request = session->get_request();
    std::size_t length{};
    length = request->get_header("Content-Length", length);
    if (length == 0)
    {
        notAcceptable(session, "Not Acceptable, empty request body");
        return;
    }

    session->fetch(
        length,
        [&db, id](const SessionPtr &session, const restbed::Bytes &data)
        {
            const std::string json{reinterpret_cast<const char *>(data.data()),
                                   data.size()};
            User user = userFromJson(json);
            if (user.name.empty() || user.id < 1 || user.email.empty() ||
                user.address.empty() || user.phone.empty())
            {
                notAcceptable(session, "Not Acceptable, invalid JSON");
                return;
            }

            {
                std::unique_lock<std::mutex> lock(g_jobdbMutex);
                db[id] = user;
            }
            session->close(restbed::OK);
        });
}

void createJob(const SessionPtr &session, jobdb &db)
{
    auto &request = session->get_request();
    std::size_t length{};
    length = request->get_header("Content-Length", length);
    if (length == 0)
    {
        notAcceptable(session, "Not Acceptable, empty body");
        return;
    }

    session->fetch(
        length,
        [&db](const SessionPtr &session, const restbed::Bytes &data)
        {
            const std::string json{reinterpret_cast<const char *>(data.data()),
                                   data.size()};
            Job job = jobFromJson(json);
            if (job.title.empty() || job.status < 0 || job.price.empty() ||
                job.description.empty() || job.location.empty() ||
                job.start_time.empty() || job.end_time.empty() ||
                job.userid < 0)
            {
                notAcceptable(session, "Not Acceptable, invalid JSON");
                return;
            }

            {
                std::unique_lock<std::mutex> lock(g_jobdbMutex);
                db.push_back(job);
            }
            session->close(restbed::OK);
        });
}


void createUser(const SessionPtr &session, userdb &db)
{
    auto &request = session->get_request();
    std::size_t length{};
    length = request->get_header("Content-Length", length);
    if (length == 0)
    {
        notAcceptable(session, "Not Acceptable, empty body");
        return;
    }

    session->fetch(
        length,
        [&db](const SessionPtr &session, const restbed::Bytes &data)
        {
            const std::string json{reinterpret_cast<const char *>(data.data()),
                                   data.size()};
            User user = userFromJson(json);
            if (user.name.empty() || user.id < 1 || user.email.empty() ||
                user.address.empty() || user.phone.empty())
            {
                notAcceptable(session, "Not Acceptable, invalid JSON");
                return;
            }

            {
                std::unique_lock<std::mutex> lock(g_jobdbMutex);
                db.push_back(user);
            }
            session->close(restbed::OK);
            //TODO Return new id
            //SET db ID to new index
            //Follow id out of range response
        });
}

void publishJobResources(restbed::Service &service, jobdb &db)
{

    // Resource for accessing jobs by id, implemented methods are GET and PUT
    auto jobResource = std::make_shared<restbed::Resource>();
    jobResource->set_path("/job/{id: [[:digit:]]+}");
    jobResource->set_method_handler("GET", [&db](const SessionPtr &session)
                                    { return readJob(session, db); });
    jobResource->set_method_handler("PUT", [&db](const SessionPtr &session)
                                    { return updateJob(session, db); });
    service.publish(jobResource);

    // Resource for creating jobs, implemented method is POST
    auto createJobResource = std::make_shared<restbed::Resource>();
    createJobResource->set_path("/job");
    createJobResource->set_method_handler("POST", [&db](const SessionPtr &session)
                                    { return createJob(session, db); });
    service.publish(createJobResource);

    // Resource for accessing jobs by user id
    auto getUserJobResource = std::make_shared<restbed::Resource>();
    getUserJobResource->set_path("/userjob/{id: [[:digit:]]+}");
    getUserJobResource->set_method_handler("GET", [&db](const SessionPtr &session)
                                    { return readUserJobs(session, db); });
    service.publish(getUserJobResource);


    // Resource for accessing filtered jobs
    auto getFilterJobResource = std::make_shared<restbed::Resource>();
    getFilterJobResource->set_path("/filteredjobs/{min: [[:digit:]]+}");
    getFilterJobResource->set_method_handler("GET", [&db](const SessionPtr &session)
                                    { return readFilteredJobs(session, db); });
    service.publish(getFilterJobResource);


    // Resource for accessing accepted jobs
    auto getAcceptedJobResource = std::make_shared<restbed::Resource>();
    getAcceptedJobResource->set_path("/acceptedjobs/{id: [[:digit:]]+}");
    getAcceptedJobResource->set_method_handler(
        "GET", [&db](const SessionPtr &session)
                                    { return readAcceptedJobs(session, db); });
    service.publish(getAcceptedJobResource);
}

void publishUserResources(restbed::Service &service, userdb &db)
{
    // Resource for creating users, implemented method is POST
    auto createUserResource = std::make_shared<restbed::Resource>();
    createUserResource->set_path("/user");
    createUserResource->set_method_handler("POST",
                                           [&db](const SessionPtr &session)
                                           { return createUser(session, db); });
    service.publish(createUserResource);

    // Resource for accessing jobs by id, implemented methods are GET and PUT
    auto userResource = std::make_shared<restbed::Resource>();
    userResource->set_path("/user/{id: [[:digit:]]+}");
    userResource->set_method_handler("GET", [&db](const SessionPtr &session)
                                    { return readUser(session, db); });
    service.publish(userResource);
}

void runService()
{
    jobdb jdb = jobLoad();
    userdb udb = userLoad();

    restbed::Service service;
    publishJobResources(service, jdb);
    publishUserResources(service, udb);
    service.set_logger(std::make_shared<CustomLogger>());
    service.start(getSettings());
}

} // namespace urepairdb

int main()
{
    try
    {
        urepairdb::runService();
    }
    catch (const std::exception &bang)
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
