#pragma once

#include <string>

namespace urepairdb
{

struct Job
{
    enum
    {
        DELETED_JOB = -1
    };

    std::string start_time;
    std::string end_time;
    int status{DELETED_JOB};
    std::string price;
    std::string description;
    std::string location;
    std::string title;
    int userid;
    int contractorid;
};

std::string toJson(const Job &job);
Job jobFromJson(const std::string &json);

} // namespace urepairdb
