#pragma once

#include <string>
#include <vector>

namespace Sidequest
{
	class Quest;

	class User {
	public:
		typedef int Id;

		User();
		~User();

		Id id;
		std::string display_name;
		std::string email;

		std::vector<Quest*> main_quests;
	};
}