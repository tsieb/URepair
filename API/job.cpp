#include "../API/jobsdb/job.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h> 

#include <restbed>

namespace urepairdb
{

    std::string toJson(const Job& job)
    {
        rapidjson::Document doc;
        rapidjson::Value& obj = doc.SetObject();
        auto addMember = [&obj, &doc](const char* key, const std::string& value)
        {
            using String = rapidjson::GenericStringRef<char>;
            obj.AddMember(String{ key }, String{ value.c_str() }, doc.GetAllocator());
        };
        addMember("start_time", job.start_time);
        addMember("end_time", job.end_time);
        obj.AddMember("status", job.status, doc.GetAllocator());
        addMember("price", job.price);
        addMember("description", job.description);
        addMember("location", job.location);
        addMember("title", job.title);
        obj.AddMember("userid", job.userid, doc.GetAllocator());
        obj.AddMember("contractorid", job.contractorid, doc.GetAllocator());
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        return buffer.GetString();
    }

    Job jobFromJson(const std::string& json)    {
        
        rapidjson::Document doc;
        doc.Parse(json.c_str());
        Job job{};
        auto getString = [&doc](const char* key) { return doc[key].GetString(); };
        job.title = getString("title");
        job.status = doc["status"].GetInt();
        job.start_time = getString("start_time");
        job.end_time = getString("end_time");
        job.price = getString("price");
        job.description = getString("description");
        job.location = getString("location");
        job.userid = doc["userid"].GetInt();
        job.contractorid = doc["contractorid"].GetInt();
        return job;
    }

} // namespace urepairdb
