#include <iostream>
#include <restbed>

#include <"URepair.h">

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace jobdb{
  string toJson(const Job& job){
  rapidjson::Document doc;  
  rapidjson::Value& obj = doc.SetObject();

  auto addMember = [&obj, &doc](const char* key, const std::string& value)
  {
    using String = rapidjson::GenericStringRef<char>;
    obj.AddMember(String{ key }, String{ value.c_str() }, doc.GetAllocator());
  };
  
  addMember("title", job.title);
  addMember("desc", job.desc);
  addMember("location", job.location);
  obj.AddMember("status", job., doc.GetAllocator());
  obj.AddMember("price", job., doc.GetAllocator());
  obj.AddMember("time", job., doc.GetAllocator());
  
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);
  return buffer.GetString();
  }
  
Job fromJson(const string& json){
 rapidjson::Document doc;
 doc.Parse(json.cstr());
 Job job{};
 auto getString = [&doc](const char* key) { return doc[key].GetString(); };
 job.title = getString("title");
 job.desc = getString("desc");
 job.location = getString("location");
 job.status = doc["status"].GetInt();
 job.price = doc["price"].GetInt();
 job.time = doc["time"].GetInt();
 return job;
}
  
} //namespace jobdb
