#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

static char *opt_host_name = "10.8.0.1";
static char *opt_user_name = "orangepi";
static char *opt_password = "0rangePi";
static unsigned opt_port_num = 3306;
static char *opt_socket_name = NULL;
static char *opt_db_name = "dts";
//static char unsigned opt_flags = 0;

static MYSQL *conn;
 
int main(int argc, char **argv){
	
  conn = do_connect(opt_host_name, opt_user_name, opt_password, opt_db_name,
                    opt_port_num, opt_socket_name, 0);

  if (conn == NULL)
    exit(1);
    
  while (1){
    char buf[10000];
    fprintf(stderr, "query> ");
    if (fgets (buf, sizeof (buf), stdin) == NULL)
      break;
    if (strcmp (buf, "quit\n") == 0 || strcmp (buf, "\\q\n") == 0)
      break;
    process_query(conn, buf);
  }
  
  do_disconnect(conn);
  
  return 0;
}
