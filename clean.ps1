docker-compose down --volumes
docker rm client-container
docker volume prune -f
docker image prune -a -f
