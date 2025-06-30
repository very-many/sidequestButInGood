#pragma once

#include <string>
#include <vector>

#include "model/user.h"
#include "network/json_serialisable.h"

namespace Sidequest
{
	class Quest;

	class SerialisableUser : public User, public JsonSerialisable {
	public:
		SerialisableUser();
		explicit SerialisableUser(Id id);
		SerialisableUser(const std::string& display_name, const std::string& email, const std::string& password);
		~SerialisableUser() override;

		// implementation of JsonSerialisable
		Json to_json(bool full_serialise = true) override;
		void from_json(const Json& json) override;
	};
}