SELECT 
    table_schema AS 'database_name',
    table_name AS 'table_name',
    table_rows AS 'row_count'
FROM information_schema.tables
WHERE table_type = 'BASE TABLE'
  AND table_rows > 0
  AND table_name NOT LIKE '%_log'
  AND table_schema NOT IN (
      'information_schema', 'performance_schema', 'sys', 'mysql'
  )
ORDER BY table_rows desc;