/*
*** error_log.cc
*** 
***
*** MySQL plugin to display errog log contents as INFORMATION.ERROR_LOG view
***
*** only valid for MySQL 5.7.x
**/

#include <sql_class.h>
#include <set_var.h> 
#include <table.h>    

#define MAX_LINE_SIZE 120

static ST_FIELD_INFO error_log_fields[]=
{
  {"LINE", MAX_LINE_SIZE, MYSQL_TYPE_STRING, 0, 0, 0, 0},
  {0, 0, MYSQL_TYPE_NULL, 0, 0, 0, 0}

};

static int error_log_fill_table(THD *thd, TABLE_LIST *tables, Item *cond)
{
  TABLE *table= tables->table;

  char *header = (char *)"reading from: ";
  char *open_error_msg = (char *)"cannot open error log";
  FILE *fd;
  char line[MAX_LINE_SIZE];

  /* first output full log err file name */
  table->field[0]->store(header, strlen(header), system_charset_info);
  if (schema_table_store_record(thd, table))
    return 1;
  table->field[0]->store(log_error_dest, strlen(log_error_dest), system_charset_info);
  if (schema_table_store_record(thd, table))
    return 1;
 
  fd = fopen(log_error_dest, "r"); 
  if (fd == NULL)
  {
	table->field[0]->store(open_error_msg, strlen(open_error_msg), system_charset_info);
	schema_table_store_record(thd, table);
    	return 1;
  }
  while (fgets(line, MAX_LINE_SIZE, fd) != NULL )
  {
	/* what if line is greater than MAX_LINE_SIZE ??? */
	table->field[0]->store(line, strlen(line), system_charset_info);
        if (schema_table_store_record(thd, table))
	    return 1;

  } 
 
  fclose(fd);

  return 0;
};

static struct st_mysql_information_schema error_log_info =
{ MYSQL_INFORMATION_SCHEMA_INTERFACE_VERSION };

static int error_log_table_init(void *ptr)
{
  ST_SCHEMA_TABLE *schema_table = (ST_SCHEMA_TABLE*)ptr;

  schema_table->fields_info = error_log_fields;
  schema_table->fill_table = error_log_fill_table;
  return 0;
};

mysql_declare_plugin(error_log)
{
  MYSQL_INFORMATION_SCHEMA_PLUGIN,
  &error_log_info,                   /* type-specific descriptor */
  "ERROR_LOG",                       /* table name */
  "Pierre Forstmann",                /* author */
  "error log",                       /* description */
  PLUGIN_LICENSE_GPL,                /* license type */
  error_log_table_init,              /* init function */
  NULL,
  0x0100,                            /* version = 1.0 */
  NULL,                              /* no status variables */
  NULL,                              /* no system variables */
  NULL,                              /* no reserved information */
  0                                  /* no flags */
}
mysql_declare_plugin_end;




