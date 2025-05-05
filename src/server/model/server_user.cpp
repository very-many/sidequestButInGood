#include "server_user.h"

namespace Sidequest 
{
	namespace Server {

		ServerUser::ServerUser( Database* database )
			: Persistable( database )
		{
		}

		ServerUser::~ServerUser()
		{
		}

	}
}