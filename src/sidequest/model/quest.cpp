#include "quest.h"

#include <utility>

namespace Sidequest {
    Quest::Quest() = default;

    Quest::Quest(const Id id) : id(id) {}

    Quest::Quest(std::string name, std::string description, Quest *parent, User *owner, User *editor)
        : name(std::move(name)), description(std::move(description)), parent(parent), owner(owner), editor(editor)
    {
    }

    Quest::~Quest() {
        for (const auto subQuest : subQuests)
            delete(subQuest);
    }

    void Quest::add_subQuest(Quest *quest) {
        subQuests.push_back(quest);
    }
}
