docker-compose down --volumes
docker volume prune -f
docker network prune -f
docker image prune -a -f
