/*	
*	Project: TimeFull DNS
*	Desc: Code to measure DNS query time and std deviation  with db operations.
*	Author: M.Waseem Sarwar (waseemsarwar103@gmail.com)
*
*/

#include "db_op.hpp"
#include "dns_op.hpp"
#include <iostream>

namespace waseem_dev
{
	typedef struct Config
	{
		unsigned int timeout_mins;
		char db_name[1000];
		char db_user[1000];
		char db_password[1000];
		char db_host[1000];
		unsigned int db_port;
	} Config;
}
