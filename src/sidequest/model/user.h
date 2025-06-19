#pragma once

#include <string>
#include <vector>

namespace Sidequest {
    class Quest;

    class User {
    public:
        typedef unsigned long Id;
        explicit User(Id);

        User(const std::string&, const std::string&, const std::string&);

        virtual ~User();

        Id id = 0;
        std::string display_name;
        std::string email;
        std::string password;

        std::vector<Quest *> main_quests;
    };
}
