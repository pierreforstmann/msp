# msp

# mySQL plugin to access error log from INFORMATION_SCHEMA.ERROR_LOG view.

For MySQL 5.5 and MySQL 5.6 you must rename error-log-5.6.cc to error-log.cc . 

For MySQL 5.7 you must rename error-log-5.7.cc to error-log.cc . 

##Build instructions

This has been tested on Centos 7.3 with gcc 4.8.5. 
The following instructions assumes you have a 5.7.19 MySQL instance running using /opt/mysql/5719 as MySQL binaries home directory.

1. clone Oracle MySQL repository and use the right branch
2. in mysql-server/plugin directory create error_log directory
3. copy from my repository error_log.cc and CMakefile.txt into plugin/error_log 
4. from mysql-server directory run `cmake . ` and `make`
5. from plugin/error_log directory run `make`
6. copy generated plugin shared object to MySQL binaries directory: 
cp `/home/mysql/mysql-server/plugin/error_log/error_log.so /opt/mysql/5719/lib/plugin/`
7. connect to MySQL instance as root to run: 
`install plugin error_log soname 'error_log.so';`
8. check that the new plugin is displayed by SHOW PLUGIN:
```
| ERROR_LOG                  | ACTIVE   | INFORMATION SCHEMA | error_log.so | GPL     |
+----------------------------+----------+--------------------+--------------+---------+
```
9. Check that INFORMATION_SCHEMA.ERROR_LOG is now displaying instance error log with:
```
mysql> select * from information_schema.error_log;
+-------------------------------------------------------------------------------------------------------------------------+
| LINE                                                                                                                    |
+-------------------------------------------------------------------------------------------------------------------------+
...
 2017-08-25T14:39:01.765573Z 0 [Note] /opt/mysql/5719/bin/mysqld: ready for connections.
                                |
| Version: '5.7.19-log'  socket: '/tmp/3316'  port: 3316  Source distribution
                                            |
| 2017-08-25T14:39:01.765588Z 0 [Note] Executing 'SELECT * FROM INFORMATION_SCHEMA.TABLES;' to get a list of tables using |
|  the deprecated partition engine. You may use the startup option '--disable-partition-engine-check' to skip this check. |
|  
                                                                                                                      |
| 2017-08-25T14:39:01.765593Z 0 [Note] Beginning of list of non-natively partitioned tables
                              |
| 2017-08-25T14:39:01.779428Z 0 [Note] End of list of non-natively partitioned tables
                                    |
| 2017-08-25T14:40:16.469866Z 3 [Note] Access denied for user 'root'@'localhost' (using password: YES)
                   |
| 2017-08-25T14:40:26.427716Z 4 [Note] Access denied for user 'root'@'localhost' (using password: YES)
                   |
+------------------------------------------------------------------------------------------------------------------
```
