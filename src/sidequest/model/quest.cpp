#include "quest.h"

namespace Sidequest {
    Quest::Quest() {
    }

    Quest::Quest(Id id) : id(id) {}

    Quest::~Quest() {
        for (const auto subquest : subquests)
            delete subquest;
    }
}
