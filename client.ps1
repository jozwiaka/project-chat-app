docker rm client-container
docker build -t client-image --build-arg DOCKER_CONTAINER=true -f docker\Dockerfile.client .
docker run --name client-container --network=my_network -it client-image