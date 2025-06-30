#include "quest.h"

#include <stdexcept>
#include <utility>
#include "user.h"

namespace Sidequest {
    Quest::Quest() = default;

    Quest::Quest(const Id id) : id(id) {
    }

    Quest::Quest(std::string title, std::string description, Quest* parent, User* owner,
                 User* editor)
        : title(std::move(title)), description(std::move(description)), parent(parent), owner(owner), editor(editor) {
        if (owner != nullptr)
            owner_id = owner->id;
        if (editor != nullptr)
            editor_id = editor->id;
        if (parent != nullptr)
            parent_id = parent->id;
    }

    Quest::Quest(std::string title, std::string description, const Status status, Quest* parent, User* owner,
                 User* editor)
        : title(std::move(title)), description(std::move(description)), status(status), parent(parent), owner(owner),
          editor(editor) {
        if (owner != nullptr)
            owner_id = owner->id;
        if (editor != nullptr)
            editor_id = editor->id;
        if (parent != nullptr)
            parent_id = parent->id;
    }

    Quest::~Quest() {
        for (const auto subQuest : subquests)
            delete(subQuest);
    }

    std::string Quest::status_to_string(Status status) {
        if (status == initial)
            return "initial";
        if (status == done)
            return "done";
        if (status == inactive)
            return "inactive";
        throw std::runtime_error("illegal status");
    }

    Quest::Status Quest::string_to_status(const std::string& status) {
        if (status == "initial")
            return initial;
        if (status == "done")
            return done;
        if (status == "inactive")
            return inactive;
        throw std::runtime_error("illegal status string");
    }

    void Quest::add_subQuest(Quest* quest) {
        subquests.push_back(quest);
    }
}
