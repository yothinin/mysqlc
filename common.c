#include <stdio.h>
#include <mysql.h>
#include "common.h"
#include <string.h>

MYSQL *
do_connect (char *host_name, char * user_name, char *password, char *db_name, 
            unsigned port_num, char *socket_name, unsigned flags)
{
  MYSQL *conn;
  conn = mysql_init(NULL);
  if (conn == NULL)
  {
    fprintf(stderr, "mysql_init() failed.\n");
    return (NULL);
  }
  if (mysql_real_connect(conn, host_name, user_name, password, 
                         db_name, port_num, socket_name, flags) == NULL)
  {
    fprintf(stderr, "mysql_real_connect() failed:\nError %u (%s)\n", 
                     mysql_errno(conn), mysql_error(conn));
    return (NULL);
  }

  return (conn);
}

void
do_disconnect(MYSQL *conn)
{
  mysql_close(conn);
}

void
process_query(MYSQL *conn, char *query)
{
  MYSQL_RES *res_set;
  unsigned field_count;
  if (mysql_query(conn, query) != 0){
    printf("process_query() failed\n");
    return ;
  }
  
  if (res_set == NULL){
    if (mysql_field_count (conn) > 0)
      printf("problem processing result set.\n");
    else
      printf("%lu rows affected\n", 
           (unsigned long) mysql_affected_rows(conn));
  }else{
    process_result_set(conn, res_set);
    //mysql_free_result(res_set);
  }
}

void
process_result_set (MYSQL *conn, MYSQL_RES *res_set)
{
  MYSQL_FIELD *field;
  MYSQL_ROW row;
  unsigned i, col_len;
  mysql_field_seek(res_set, 0);
  
  for (i = 0; i < mysql_num_fields (res_set); i++){
    field = mysql_fetch_field(res_set);
    col_len = strlen(field->name);
    if (col_len < field->max_length)
      col_len = field->max_length;
    if (col_len < 4 && IS_NOT_NULL(field->flags))
      col_len = 4;
    field->max_length = col_len;
  }
  print_dashes(res_set);
  fputc ('|', stdout);
  mysql_field_seek(res_set, 0);
  
  for (i = 0; i < mysql_num_fields (res_set); i++){
    field = mysql_fetch_field(res_set);
    printf(" %-*s |", field->max_length, field->name);
  }
  fputc('\n', stdout);
  print_dashes(res_set);
  
  while ((row = mysql_fetch_row (res_set)) != NULL){
    mysql_field_seek(res_set, 0);
    fputc ('|', stdout);
    for (i = 0; i < mysql_num_fields(res_set); i++){
      field = mysql_fetch_field(res_set);
      if (row[i] == NULL)
        printf(" %-*s |", field->max_length, "NULL");
      else if (IS_NUM(field->type))
        printf(" %*s |", field->max_length, row[i]);
      else
        printf(" %-*s |", field->max_length, row[i]);
	}
	fputc('\n', stdout);
  }
  print_dashes(res_set);
  printf("%lu rows returned\n", (unsigned long) mysql_num_rows(res_set));
  
}

void
print_dashes (MYSQL_RES *res_set)
{
  MYSQL_FIELD *field;
  unsigned i, j;
  mysql_field_seek(res_set,0);
  fputc('+', stdout);
  for (i = 0; i < mysql_num_fields(res_set); i++){
    field = mysql_fetch_field (res_set);
    for (j = 0; j < field->max_length + 2; j++)
      fputc ('-', stdout);
    fputc('+', stdout);
  }
  fputc('\n', stdout);
}
