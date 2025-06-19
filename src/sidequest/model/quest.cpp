#include "quest.h"

#include <stdexcept>
#include <utility>

namespace Sidequest {
    Quest::Quest() = default;

    Quest::Quest(const Id id) : id(id) {
    }

    Quest::Quest(std::string name, std::string description, Quest* parent, User* owner,
                 User* editor)
        : name(std::move(name)), description(std::move(description)), parent(parent), owner(owner), editor(editor) {
    }

    Quest::Quest(std::string name, std::string description, Quest::Status status, Quest* parent, User* owner,
                 User* editor)
        : name(std::move(name)), description(std::move(description)), status(status), parent(parent), owner(owner),
          editor(editor) {
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
