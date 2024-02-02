& .\Clean.ps1
docker-compose build
docker-compose up -d postgres-service server-service
docker-compose run --rm client-service