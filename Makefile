CXX=g++
INCLUDE=-I/usr/local/include/mysql++/ -I/usr/include/mysql/
LINK=-lmysqlpp -lldns

dns_query_time.o: main.cpp db_op.cpp *.hpp
	$(CXX) main.cpp db_op.cpp $(INCLUDE) $(LINK) -o dns_query_time.o

clean:
	rm -r dns_query_time.o
