#include "user.h"

namespace Sidequest {
    User::User() = default;

    User::User(const Id id) : id(id) {
    }

    User::User(const std::string& display_name, const std::string& email, const std::string& password)
        : display_name(display_name), email(email), password(password) {
    }

    User::User(std::string&& display_name, std::string&& email, std::string&& password)
        : display_name(std::move(display_name)), email(std::move(email)), password(std::move(password)) {
    }

    User::~User() = default;
}
