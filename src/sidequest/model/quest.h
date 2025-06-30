#pragma once

#include <optional>
#include <string>
#include <vector>
#include "id.h"

namespace Sidequest {
    class User;

    class Quest {
    public:
        enum Status {
            initial,
            done,
            inactive
        };

        Quest();
        explicit Quest(Id id);
        Quest(std::string title, std::string description, Quest* parent, User* owner, User* editor);
        Quest(std::string title, std::string description, Status status, Quest* parent, User* owner, User* editor);
        virtual ~Quest();

        static std::string status_to_string(Status status);
        static Status string_to_status(const std::string& status);

        void add_subQuest(Quest* quest);

        Id id = 0;
        std::string title;
        std::string description;
        Status status = initial;
        std::vector<Quest *> subquests;

        std::optional<Id> parent_id;
        Quest* parent = nullptr;

        std::optional<Id> owner_id;
        User* owner = nullptr;

        std::optional<Id> editor_id;
        User* editor = nullptr;
    };
}
