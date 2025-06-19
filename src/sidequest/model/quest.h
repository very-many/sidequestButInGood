#pragma once

#include <string>
#include <vector>

namespace Sidequest {
    class User;

    class Quest {
    public:
        typedef unsigned long Id;
        enum Status {
            initial,
            done,
            inactive
        };

        Quest();
        explicit Quest(Id id);
        Quest(std::string name, std::string description, Quest* parent, User* owner, User* editor);
        Quest(std::string name, std::string description, Status status, Quest* parent, User* owner, User* editor);

        virtual ~Quest();

        static std::string status_to_string(Status status);
        static Status string_to_status(const std::string& status);

        void add_subQuest(Quest* quest);

        Id id = 0;
        std::string name;
        std::string description;
        Status status = initial;
        Quest* parent = nullptr;

        User* owner = nullptr;
        User* editor = nullptr;

        std::vector<Quest *> subquests;
    };
}
