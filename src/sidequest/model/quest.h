#pragma once

#include <string>
#include <vector>

namespace Sidequest {
    class User;

    class Quest {
    public:
        typedef unsigned long Id;

        Quest();
        explicit Quest(Id id);
        Quest(std::string name, std::string description, Quest* parent, User* owner, User* editor);

        virtual ~Quest();

        void add_subQuest(Quest* quest);

        Id id = 0;
        std::string name;
        std::string description;
        Quest* parent = nullptr;

        User* owner = nullptr;
        User* editor = nullptr;

        std::vector<Quest *> subQuests;
    };
}
