#!/bin/bash

# Constant
kUsage="
Usage: $(basename $0) [OPTIONS] \n

Options: \n
  run \t\t Run the container. If not exist, create first. If exitsted, reattach. \n
  delete \t Delete current container in order to create a new one. Use it when you encounter some problems. ATTENTION: data written to places other then the workspace folder will be deleted! \n
  gpu \t\t if you can run gpu version of this docker, such as: bash ./scripts/run_container run gpu\n"

CONTAINER_NAME="bme4030-project-cpu"
IMAGE_NAME="khjtony/bme4030-project-cpu:dev"

# arguments
opt_run=false
opt_delete=false
if [[ $# -gt 0 ]]; then
  if [[ $1 == "run" ]]; then
    opt_run=true
  elif [[ $1 == "delete" ]]; then
    opt_delete=true
  fi
else
  echo -e $kUsage
  exit;
fi

if [[ $2 = "flir_transponder" ]]; then
  CONTAINER_NAME="flir_transponder_15000_20000"
  IMAGE_NAME="khjtony/flir_transponder:latest"
elif [[ $2 = "flir_client_x86" ]]; then
  CONTAINER_NAME="flir_client_x86"
  IMAGE_NAME="khjtony/flir_client_x86:latest"
elif [[ $2 = "flir_client_aarch64" ]]; then
  CONTAINER_NAME="flir_client_aarch64"
  IMAGE_NAME="khjtony/flir_client_aarch64:latest"
else
  echo Please specify image to run
  exit;
fi

# Check if container already there
if [ `docker container ls -a | grep ${CONTAINER_NAME} | wc -l` -ge 1 ]; then
  container_id=`docker container ls -a | grep ${CONTAINER_NAME} | cut -d' ' -f1`
  if $opt_run ; then
    echo "There is a container. Reattach by default"
    docker container start $container_id
    docker container attach $container_id
    exit;
  elif $opt_delete ; then
    echo "Delete current container. You need to run a new one"
    docker container stop $container_id
    docker container rm $container_id
    exit;
  fi
else
  if $opt_run ; then
    echo "Container ${CONTAINER_NAME} not found. Will create a new one."
  elif $opt_delete ; then
    echo "Container ${CONTAINER_NAME} not found. Will do nothing."
    exit;
  fi
fi


# Open port for Jupyter. Default is 8888.
docker run -d \
  --name $CONTAINER_NAME \
  --restart always \
  -e LD_LIBRARY_PATH=/bin:$LD_LIBRARY_PATH \
  -e ENV_BIND_IP=128.59.246.245 \
  --privileged \
  -v /dev:/dev \
  --network=host \
  $IMAGE_NAME
