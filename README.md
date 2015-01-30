# TIMEFUL-DNS

Author: M.Waseem Sarwar
Email: waseemsarwar103@gmail.com

C++ Project to query DNS to any number of domains and save average query time, standard deviation and number of queries made in mysql database.

Requirements: 

a. Mysql lib, use mysql++:
http://tangentsoft.net/mysql++/
b. DNS lib, use ldns:
http://www.nlnetlabs.nl/projects/ldns/

Work-Flow:

1. The code reads the list of URL's from the static url array defined in the main.cpp file (not reading URLs from DB) as my code assumes that there may or maynot be DB present at the time of execution. Add or remove URL's from the static array and recompile. 

2. On execution, the code will create the DB (if doesn't exist) and setup necessary tables. This will persist if the code is run multiple times and query values will be correct all the times.

3. We calculate the required parameters and save them in the DB. The Db name is provided via CLI and table name is 'domain_query_times'. All the timing information (mean, std_deviation) is saved in milliseconds. Please ignore DB columns staring with 'calc_' as they are used for saving some internal value while calculations. 

4. I have not appended random string in front of DNS queries as I can't resolve DNS if I append random string at front. I might be missing something here but not sure ?

5. Run 'make' to compile the code. Fix include and linker paths if they are at different location.

6. To execute, run the code with following required and optional params,
  ./dns_query_time.o -t <timeout_mins> -d <db_name> -u <user> -p <passwd> -P <port_optional> -h <host_optional>

7. I have tried to add some comments where certain thing could be optimized and I didn't do it for sake of simplicity. 

8. A sample output from the DB from one of my execution is shown as follows, 
mysql> select * from domain_query_times;
+---------------+---------------------+---------------------+-------------+--------+---------------+--------------+----------+
| domain_name   | last_timestamp      | first_timestamp     | num_queries | mean   | std_deviation | calc_std_dev | calc_sum |
+---------------+---------------------+---------------------+-------------+--------+---------------+--------------+----------+
| baidu.com     | 2015-01-20 00:35:49 | 2015-01-20 00:31:43 |           5 |  70.41 |         17.72 |     26360.20 |   352.06 |
| blogger.com   | 2015-01-20 00:35:49 | 2015-01-20 00:31:43 |           5 |  70.46 |          2.24 |     24854.90 |   352.32 |
| facebook.com  | 2015-01-20 00:35:48 | 2015-01-20 00:31:42 |           5 |  58.85 |          5.48 |     17471.30 |   294.25 |
| google.com    | 2015-01-20 00:35:48 | 2015-01-20 00:31:42 |           5 | 376.09 |        394.36 |   1484830.00 |  1880.45 |
| live.com      | 2015-01-20 00:35:49 | 2015-01-20 00:31:43 |           5 |  54.96 |          3.32 |     15160.40 |   274.82 |
| msn.com       | 2015-01-20 00:35:49 | 2015-01-20 00:31:43 |           5 |  67.02 |          3.46 |     22521.90 |   335.09 |
| qq.com        | 2015-01-20 00:35:49 | 2015-01-20 00:31:43 |           5 | 228.89 |        332.92 |    816112.00 |  1144.43 |
| wikipedia.org | 2015-01-20 00:35:49 | 2015-01-20 00:31:43 |           5 |  77.23 |         25.86 |     33168.10 |   386.15 |
| yahoo.com     | 2015-01-20 00:35:48 | 2015-01-20 00:31:42 |           5 |  58.28 |          8.77 |     17369.00 |   291.40 |
| youtube.com   | 2015-01-20 00:35:48 | 2015-01-20 00:31:42 |           5 |  73.97 |          3.46 |     27417.40 |   369.83 |
+---------------+---------------------+---------------------+-------------+--------+---------------+--------------+----------+
10 rows in set (0.00 sec)
