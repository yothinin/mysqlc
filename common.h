MYSQL *
do_connect (char *host_name, char * user_name, char *password, char *db_name, 
            unsigned portnum, char *socket_name, unsigned flags);

void
do_disconnect(MYSQL *conn);
void
process_query(MYSQL *conn, char *query);
void
process_result_set (MYSQL *conn, MYSQL_RES *res_set);
void
print_dashes (MYSQL_RES *res_set);
