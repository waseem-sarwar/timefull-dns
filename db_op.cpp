/*	
*	Project: TimeFull DNS
*	Desc: Code to measure DNS query time and std deviation  with db operations.
*	Author: M.Waseem Sarwar (waseemsarwar103@gmail.com)
*
*/

#include "db_op.hpp"

namespace waseem_dev
{

bool
DbOps::insert(const char *domain, double query_time)
{
	mysqlpp::Query query = m_conn->query();
	query << "USE " << m_db_name;
	query.execute();
	query << "SELECT * FROM domain_query_times WHERE domain_name = '" << domain << "'";
	mysqlpp::StoreQueryResult res = query.store();
		
	if (res.num_rows() == 0)
	{
		double calc_std_dev = query_time * query_time;
		query << "INSERT INTO domain_query_times "
			"(domain_name, first_timestamp, last_timestamp, num_queries, std_deviation, "
			"calc_std_dev, calc_sum, mean) VALUE ('" << domain << "', CURRENT_TIMESTAMP, NOW(), '1', '0', '" 
			<< double(calc_std_dev) <<"', '" << query_time << "', '" << query_time << "')";
		query.execute(); 
	}
	else
	{
		// Can't have more than one entries for primary key.
		// Formula for std-dev has been taken from 
		// http://en.wikipedia.org/wiki/Standard_deviation#Identities_and_mathematical_properties
		mysqlpp::StoreQueryResult::iterator i;
		const mysqlpp::Row& row = *(res.begin());
		double calc_std_dev = row[6];
		calc_std_dev += query_time * query_time;

		double calc_sum = row[7];
		calc_sum += query_time;

		unsigned int num_queries = row[3];
		++num_queries;

		double mean = (double)(calc_sum / num_queries);
		
		double std_dev = sqrt(abs((calc_std_dev/num_queries) - (mean * mean)));

		query << "UPDATE domain_query_times SET last_timestamp=NOW(), num_queries='" << num_queries 
		<< "', std_deviation='" << double(std_dev) << "', calc_std_dev='" << double(calc_std_dev) << "', calc_sum='" 
		<< double(calc_sum) << "', mean='" << double(mean) << "' where domain_name='" << domain << "'";
		query.execute();
	}
}

};
