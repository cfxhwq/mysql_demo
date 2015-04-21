#include <stdio.h>
#include <iostream>
#include "mysql.h"
#include <string>
#include <cstring>
#include <strings.h>
#include <cstdlib>
using namespace std;

int executeSQL(MYSQL &mysql, string sql);
void queryANDprint(MYSQL &mysql, string sql);

main() 
{
	MYSQL mysql;

	if(mysql_init(&mysql))
	  printf("mysql init finish!\n");
	else{
		printf("mysql init error!\n");
		exit(-1);
	}
	const char *IP		 = "localhost";
	const char *USERNAME = "root";
	const char *PASSWORD = "root";
	unsigned int PORT	 = 3306;	
	const char *DB_NAME  = "testDB";

	if(mysql_real_connect(&mysql, IP, USERNAME , PASSWORD, DB_NAME, PORT, NULL, 0))
	  printf("mysql connect finish!\n");
	else{
		printf("mysql connect error %d:%s\n", mysql_errno(&mysql),mysql_error(&mysql));	  
		mysql_close(&mysql);
		exit(-1);
	}

	string sql = "create table seu(id int)";

	//if(executeSQL(mysql, sql) >= 0)
	//printf("SQL :%s\n",sql);
	//else{
	//mysql_close(&mysql);
	//exit(-1);
	//}

	//unsigned long res = (unsigned long)mysql_affected_rows(&mysql);
	//cout<< res <<endl;

	sql = "select * from seu";
	queryANDprint(mysql, sql); 

	cout<< "client lib version:\t" << mysql_get_client_info() <<endl;
	cout<< "server connection info\t" << mysql_get_host_info(&mysql) <<endl;
	cout<< "connected server info:\t" << mysql_get_server_info(&mysql) <<endl;
	cout<< "is it success in last command:\t" << (mysql_info(&mysql) != NULL) <<endl;

	cout<< "chang DB:\t" << mysql_select_db(&mysql, "otherDB") <<endl;//success==0
	cout<< "shutdown server:\t" << mysql_shutdown(&mysql, SHUTDOWN_DEFAULT) <<endl;//success==0

	mysql_close(&mysql);
}

int executeSQL(MYSQL &mysql, string sql){
	if(mysql_query(&mysql, sql.c_str()) == 0)
	  printf("mysql executeSQL finish!\n");
	else{
		printf("mysql executeSQL error %d:%s\n", mysql_errno(&mysql),mysql_error(&mysql));	  
		return -1;	
	}
	return 0;
}

void queryANDprint(MYSQL &mysql, string sql){
	if(executeSQL(mysql, sql) >= 0)
	  printf("SQL : %s\n", sql.c_str());
	else{
		printf("mysql executeSQL error %d:%s\n", mysql_errno(&mysql),mysql_error(&mysql));	  
		mysql_close(&mysql);
		exit(-1);
	}
	MYSQL_RES *res = mysql_store_result(&mysql);//mysql_use_result(&mysql);
	MYSQL_ROW sqlrow;
	int first_row = 1;
	if(res){
		printf("get %lu rows\n", (unsigned long)mysql_num_rows(res));
		while(sqlrow = mysql_fetch_row(res)){
			if(first_row){
				printf("field count:\t%d\n", mysql_field_count(&mysql));
				MYSQL_FIELD *header;
				printf("------------------------------------------\n");
				while((header = mysql_fetch_field(res)) != NULL){
					printf("Name:\t%s\n",header->name);
					printf("def:\t%s\n",header->def);
					printf("table:\t%s\n",header->table);
					printf("length:\t%ld\n",header->length);
					printf("max length:\t%ld\n",header->max_length);
					printf("flags:\t%d\n",header->flags);
					printf("decimals:%d\n",header->decimals);
				
					printf("type:\t");
					if(IS_NUM(header->type)){
						printf("is num\n");
					}
					else{
						switch(header->type){
							case FIELD_TYPE_VAR_STRING:printf("VARCHAR\n");break;
							case FIELD_TYPE_STRING:printf("CHAR\n");break;
							case FIELD_TYPE_LONG:printf("LONG\n");break;
							case FIELD_TYPE_INT24:printf("INT\n");break;
							//and so on
							default:printf("unknow\n");
						}
					}
					printf("decimals:%d\n",header->decimals);
					printf("------------------------------------------\n");
				}
				first_row = 0;
			}

			unsigned int field_count = 0;
			while(field_count < mysql_field_count(&mysql)){
				if(sqlrow[field_count])
				  printf("%s\t",sqlrow[field_count]);
				else
				  printf("NULL");
				field_count++;
			}
			printf("\n");
		}
		mysql_free_result(res);
	}	
	else{
		printf("mysql query error %d:%s\n", mysql_errno(&mysql),mysql_error(&mysql));	  
		mysql_close(&mysql);
		exit(-1);
	}
}
