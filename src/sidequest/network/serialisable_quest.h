#pragma once

#include <string>
#include <vector>

#include "model/quest.h"
#include "network/json_serialisable.h"

namespace Sidequest {
    class User;

    NLOHMANN_JSON_SERIALIZE_ENUM(Quest::Status,
                                 {
                                 { Quest::Status::initial, "initial" },
                                 { Quest::Status::done, "done" },
                                 { Quest::Status::inactive, "inactive" }
                                 })

    class SerialisableQuest : public Quest, public JsonSerialisable {
    public:
        SerialisableQuest();
        explicit SerialisableQuest(Id id);
        SerialisableQuest(std::string title, std::string description, Status status, Quest* parent, User* owner,
                          User* editor);
        ~SerialisableQuest() override;

        // implementation of JsonSerialisable
        Json to_json(bool full_serialise = true) override;
        void from_json(const Json& json) override;

    public:
        bool serialise_parent_recursive = false;
        bool serialise_owner_recursive = false;
        bool serialise_editor_recursive = false;
        bool serialise_subquests_recursive = false;
    };
}
