file1 = mysql.cpp

run:$(file1)
	g++ -o $@ $< \
		-lmysqlclient \
		-I/usr/include/mysql \
		-L/usr/lib/mysql
