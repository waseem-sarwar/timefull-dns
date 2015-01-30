/*	
*	Project: TimeFull DNS
*	Desc: Code to measure DNS query time and std deviation  with db operations.
*	Author: M.Waseem Sarwar (waseemsarwar103@gmail.com)
*
*/

#include <mysql++.h>
#include <ssqls.h>
#include <connection.h>

namespace waseem_dev
{

using namespace std;

// Could have make this as singleton object but not necessary for our implementation
class DbOps
{
public:
	DbOps(const char *db_name, const char *db_user, const char *db_host, const char *db_password, unsigned int db_port)
	{
		try
		{
			m_conn = new mysqlpp::Connection(0, db_host, db_user, db_password, db_port);
			mysqlpp::Query query = m_conn->query();

			
			query << "CREATE DATABASE IF NOT EXISTS " << db_name;
			query.execute();
	
			query << "USE " << db_name;
			query.execute();
	
			// Columns with 'calc_' prefix are just used for internal calculations.
			query << "CREATE TABLE IF NOT EXISTS domain_query_times (" <<
				 " domain_name CHAR(255) NOT NULL, "
				 " last_timestamp  TIMESTAMP  NOT NULL, "
				 " first_timestamp TIMESTAMP NOT NULL, "
				 " num_queries BIGINT  NOT NULL, "
				 " mean FLOAT(10,2) NOT NULL, "
				 " std_deviation FLOAT(10,2) NOT NULL, "
				 " calc_std_dev FLOAT(10,2)  NOT NULL, "
				 " calc_sum FLOAT(10,2) NOT NULL, "
				 " PRIMARY KEY (domain_name)"
				 ")";
			query.execute();

			m_db_name = db_name;
		}
		catch (const mysqlpp::BadQuery &er)
		{
			cout << "Bad Query to DB: " << er.what() << endl;
			cout << "Aborting " << endl;
			exit(1);
		}
		catch (const mysqlpp::ConnectionFailed &er)
		{
			cout << "Connection can't be created to DB: " << er.what() << endl;
			cout << "Aborting " << endl;
			exit(1);
		}
	}

	bool insert(const char *domain, double query_time);
private:
	mysqlpp::Connection *m_conn;
	const char *m_db_name;
};

}
