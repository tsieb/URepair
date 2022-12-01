#include "../user.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <restbed>

namespace urepairdb
{

std::string toJson(const User &user)
{
    rapidjson::Document doc;
    rapidjson::Value &obj = doc.SetObject();
    auto addMember = [&obj, &doc](const char *key, const std::string &value)
    {
        using String = rapidjson::GenericStringRef<char>;
        obj.AddMember(String{key}, String{value.c_str()}, doc.GetAllocator());
    };
    obj.AddMember("id", user.id, doc.GetAllocator());
    addMember("phone", user.phone);
    addMember("address", user.address);
    addMember("email", user.email);
    addMember("name", user.name);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

User userFromJson(const std::string &json)
{
    rapidjson::Document doc;
    doc.Parse(json.c_str());
    User user{};
    auto getString = [&doc](const char *key) { return doc[key].GetString(); };
    user.id = doc["id"].GetInt();
    user.phone = getString("phone");
    user.address = getString("address");
    user.email = getString("email");
    user.name = getString("name");
    return user;
}

} // namespace urepairdb
