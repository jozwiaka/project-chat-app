DROP DATABASE IF EXISTS db_chat;
DROP USER IF EXISTS jozwiaka;

CREATE USER jozwiaka WITH PASSWORD 'password';
CREATE DATABASE db_chat WITH OWNER = jozwiaka;
GRANT ALL PRIVILEGES ON DATABASE db_chat to jozwiaka;

SELECT usename FROM pg_user WHERE usename = 'jozwiaka';
