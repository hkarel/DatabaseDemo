Это демонстрационный проект. Он содержит примеры использования [драйверов](https://github.com/hkarel/Database)  для работы с базами данных. Драйверы разработаны под QtFramework. Репозиторий содержит примеры для трех СУБД: FireBird, Postgres, MS SQL.   
Проект создан с использованием QtCreator, сборочная система QBS. Есть четыре сборочных сценария:

1. db_demo_project.qbs - демо примеры для всех СУБД (содержит пункты 2-4);
2. db_demo_firebird.qbs - демо пример для FireBird (требуется FireBird-клиент);
3. db_demo_postgres.qbs - демо пример для Postgres (требуется пакет libpq-dev);
4. db_demo_mssql.qbs - демо пример для MS SQL.

Драйвера в первую очередь разрабатывались для работы в Linux, поэтому эксплуатационное  тестирование выполнялось именно для этой ОС. В Windows будет работать FireBird-драйвер (проверено), для остальных драйверов тестирование не проводилось. 

Демо-примеры записывают следующие логи:

- /tmp/db-demo-firebird.log
- /tmp/db-demo-mssql.log
- /tmp/db-demo-postgres.log

При первом запуске примеры проверяют наличие тестовой базы данных. Если базы не обнаружено в лог-файл будет выведен скрипт для ее создания.

Установка ODBC драйвера для MS SQL ([источник](https://learn.microsoft.com/en-us/sql/connect/odbc/linux-mac/installing-the-microsoft-odbc-driver-for-sql-server?view=sql-server-ver16&tabs=ubuntu18-install%2Cubuntu17-install%2Cdebian8-install%2Credhat7-13-install%2Crhel7-offline)):  
1) `curl https://packages.microsoft.com/keys/microsoft.asc | sudo tee /etc/apt/trusted.gpg.d/microsoft.asc`   
2) `curl https://packages.microsoft.com/config/ubuntu/$(lsb_release -rs)/prod.list | sudo tee /etc/apt/sources.list.d/mssql-release.list`   
3) `sudo apt-get update`  
4) `sudo ACCEPT_EULA=Y apt-get install -y msodbcsql18`  
5) `sudo ACCEPT_EULA=Y apt-get install -y unixodbc-dev`

