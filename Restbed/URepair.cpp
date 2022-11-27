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
  obj.AddMember("status", job.status, doc.GetAllocator());
  obj.AddMember("price", job.price, doc.GetAllocator());
  obj.AddMember("time", job.time, doc.GetAllocator());
  
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

namespace userdb{
  string toJson(const User& user){
  rapidjson::Document use_doc;  
  rapidjson::Value& obj = use_doc.SetObject();

  auto addMember = [&obj, &use_doc](const char* key, const std::string& value)
  {
    using String = rapidjson::GenericStringRef<char>;
    obj.AddMember(String{ key }, String{ value.c_str() }, use_doc.GetAllocator());
  };
  
  addMember("username", user.username);
  addMember("name", user.name);
  addMember("address", user.address);
  addMember("email", user.email);
  addMember("phone", user.phone);
  addMember("type", user.type);
  
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);
  return buffer.GetString();
  }
  
User fromJson(const string& json){
 rapidjson::Document use_doc;
 use_doc.Parse(json.cstr());
 User user{};
 auto getString = [&use_doc](const char* key) { return use_doc[key].GetString(); };
 user.username = getString("title");
 user.name = getString("desc");
 user.address = getString;
 user.email = getString;
 user.phone = getString;
 user.type = getString;
 return user;
}
  
} //namespace userdb
