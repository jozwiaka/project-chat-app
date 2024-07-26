install:
	apt update && apt upgrade -y
	apt install -y postgresql postgresql-contrib libpq-dev libpqxx-dev
	systemctl is-active postgresql || service postgresql start

edit_config:
	#set no password for superuser called 'postgres'
	sed -i 's/\(local[[:space:]]\+all[[:space:]]\+postgres[[:space:]]\+\)md5/\1trust/' /etc/postgresql/*/main/pg_hba.conf

	#change peer to md5
	sed -i 's/peer/md5/g' /etc/postgresql/*/main/pg_hba.conf

	service postgresql restart

create:
	psql -U postgres -f create_user_and_db.sql
	psql -U jozwiaka -d db_chat -a -f create_tables.sql
	# psql -U jozwiaka -l

all: install edit_config create

	
