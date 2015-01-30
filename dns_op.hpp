/*	
*	Project: TimeFull DNS
*	Desc: Code to measure DNS query time and std deviation  with db operations.
*	Author: M.Waseem Sarwar (waseemsarwar103@gmail.com)
*
*/

#include <ldns/ldns.h>

namespace waseem_dev
{

using namespace std;

class DnsOps
{
public:
	double resolve(const char *domain_name)
	{
		timeval tv_base, tv_done;
		
		// For more reliability, I could have used pthread_setaffinity but I am skipping it for now to 
		// avoid complexity.
		gettimeofday(&tv_base, NULL);

		// Not appending a random string in domain name as the name resolution fails !!
		pthread_create(&m_thread, NULL, &DnsOps::entry_point, (void *)domain_name);
		pthread_join(m_thread, NULL);
		gettimeofday(&tv_done, NULL);
	
		return time_diff(&tv_done, &tv_base);
	}
private:
	pthread_t m_thread;
	char *domain_name;

	static void *entry_point(void *ptr)
	{
		const char *domain_name = static_cast<const char *>(ptr);
		ldns_resolver *res;
		ldns_rdf *name;
		ldns_status status;
		ldns_rr_list *addr;		

		name = ldns_dname_new_frm_str(domain_name);

		// Should add more debug prints.
		if (!name)
			return NULL;

		// Resolve using resolv.conf.
		status = ldns_resolver_new_frm_file(&res, NULL);
		if (status != LDNS_STATUS_OK) 
		{
			ldns_rdf_deep_free(name);
			return NULL;
		}
		ldns_resolver_set_retry(res, 2);

		addr = ldns_get_rr_list_addr_by_name(res, name, LDNS_RR_CLASS_IN, LDNS_RD);
		if (!addr) 
		{
			ldns_rdf_deep_free(name);
			ldns_resolver_deep_free(res);
			return NULL;
		}

		return NULL;
	}

	// Returning msecond time
	static double time_diff(timeval *end, timeval *start)
	{
		double ret =  (double)((unsigned long)((end->tv_sec - start->tv_sec) * 1000000) +
			(end->tv_usec - start->tv_usec));
		return (ret ? ret  / 1000 : ret);
	}

};
}
