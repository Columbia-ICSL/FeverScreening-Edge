#!/bin/bash
if [[ $1 = "flir_transponder" ]]; then
  mkdir .temp_dockerbuild
  bazel build //:flir_transponder_pkg
  cp bazel-bin/flir_transponder_pkg.tar ./.temp_dockerbuild
  image_name="khjtony/flir_transponder:latest"
  docker build -t $image_name -f docker/Dockerfile.transponder .
  rm -rf ./.temp_dockerbuild
elif [[ $1 = "flir_client_x86" ]]; then
  mkdir .temp_dockerbuild
  bazel build //:flir_client_x86_pkg
  cp bazel-bin/flir_client_x86_pkg.tar ./.temp_dockerbuild
  image_name="khjtony/flir_client_x86:latest"
  docker build -t $image_name -f docker/Dockerfile.flir_client .
  rm -rf ./.temp_dockerbuild
elif [[ $1 = "flir_client_aarch64" ]]; then
  mkdir .temp_dockerbuild
  bazel build //:flir_client_aarch64_pkg
  cp bazel-bin/flir_client_aarch64_pkg.tar ./.temp_dockerbuild
  image_name="khjtony/flir_client_aarch64:latest"
  docker build -t $image_name -f docker/Dockerfile.flir_client_aarch64 .
  rm -rf ./.temp_dockerbuild

else
  echo Please specify image to build
  exit;
fi
