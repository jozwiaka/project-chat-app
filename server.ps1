& .\Clean.ps1
docker-compose build
docker network create my_network
docker-compose up -d postgres-service server-service
