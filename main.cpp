/*	
*	Project: TimeFull DNS
*	Desc: Code to measure DNS query time and std deviation  with db operations.
*	Author: M.Waseem Sarwar (waseemsarwar103@gmail.com)
*
*/

#include "main.hpp"

using namespace std;
using namespace waseem_dev;

// Declaring global config struct.
Config g_config;

// Add more domains here. Can easily be migrated to external txt file.
static const char * s_domain_names[] = {"google.com",
					"facebook.com",
					"youtube.com",
					"yahoo.com",
					"live.com",
					"wikipedia.org",
					"baidu.com",
					"blogger.com",
					"msn.com",
					"qq.com",		
					NULL
					};

static void print_usage(void)
{
	cout << "USAGE:  ./dns_query_time.o -t <timeout_mins> -d <db_name> -u <user> -p <passwd> -P <port_optional> -h <host_optional>" << endl;
	exit(0);
}

int main(int argc, char* argv[])
{
	char c;
	memset(&g_config, 0, sizeof(g_config));
	strcpy(g_config.db_host, "localhost");
	g_config.db_port = 3306;
	g_config.timeout_mins = 0;
	
	while ((c = getopt(argc, argv, "t:d:u:p:h:P:")) != -1)
	{
		switch(c)
		{
		case 't':
			g_config.timeout_mins = atoi(optarg);
			break;
		case 'd':
			strcpy(g_config.db_name, optarg);
			break;
		case 'u':
			strcpy(g_config.db_user, optarg);
			break;
		case 'p':
			strcpy(g_config.db_password, optarg);
			break;
		case 'P':
			g_config.db_port = atoi(optarg);
			break;
		case 'h':
			strcpy(g_config.db_host, optarg);
			break;
		default:
			print_usage();
			break;
		}
	}

	if (strcmp(g_config.db_name, "") == 0 || strcmp(g_config.db_user, "") == 0 || strcmp(g_config.db_password, "") == 0 || g_config.timeout_mins == 0)
		print_usage();

	DbOps *db = new DbOps(g_config.db_name, g_config.db_user, g_config.db_host, g_config.db_password, g_config.db_port);
	
	// Run Infinitely
	while (true)
	{
		cout << "Executing Domain Name Resolution " << endl;
		const char **domain_name = s_domain_names;
		// Could run queries in parallel threads with barrier sync. Doesn't seem necessary for this code. 
		for (; *domain_name != NULL; domain_name++)
		{
			cout << "Resolving: " << *domain_name << endl;
			DnsOps dns;
			double query_time = dns.resolve(*domain_name);
			db->insert(*domain_name, query_time);
		}
		cout << "Sleeping for " << g_config.timeout_mins << " minutes" << endl;
		sleep(g_config.timeout_mins * 60);
	}

	return 0;
}
