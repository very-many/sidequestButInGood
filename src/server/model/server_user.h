#pragma once

#include <string>
#include <vector>

#include <model/user.h>

namespace Sidequest
{
	namespace Server
	{

		class ServerUser : public Sidequest::User {
		public:
			typedef unsigned long Id;

			ServerUser( Database* database );
			~ServerUser();

		};

	}
}