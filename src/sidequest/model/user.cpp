#include "user.h"

#include <utility>

namespace Sidequest {
    User::User(const Id id) : id(id) {
    }

    User::User(const std::string& display_name, const std::string& email, const std::string& password)
        : display_name(display_name), email(email), password(password) {
    }

    User::~User() = default;
}
