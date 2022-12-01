#pragma once

#include <string>

namespace urepairdb
{

    struct User
    {
        enum
        {
            DELETED_USER = -1
        };
        //Either 0 for normal or -1 if deleted
        // Not actually representative of id
        int id{ DELETED_USER };
        std::string phone;
        std::string address;
        std::string email;
        std::string name;
    };

    std::string toJson(const User& user);
    User userFromJson(const std::string& json);

}
