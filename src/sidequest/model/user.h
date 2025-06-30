#pragma once

#include <string>
#include <vector>
#include "id.h"

namespace Sidequest {
    class Quest;

    class User {
    public:
        User();
        explicit User(Id);

        User(const std::string& display_name, const std::string& email, const std::string& password);
        User(std::string&& display_name, std::string&& email, std::string&& password);

        virtual ~User();

        Id id = 0;
        std::string display_name;
        std::string email;
        std::string password;

        std::vector<Quest *> main_quests;
    };
}
