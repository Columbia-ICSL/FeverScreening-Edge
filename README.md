# MULTI-PERSON FEVER SCREENING

## About
This repository is the program for the fever screening edge device. The edge device consists of a <a href="https://www.amazon.com/FLIR-Thermal-Imaging-Microfiber-Cleaning/dp/B07NS5WNRD/" target="_blank">Flir One Pro</a> and a <a href="https://usedipaq.com/products/nvidia-jetson-nano-developer-kit-2gb" target="_blank">Jetson Nano </a> are required.

The program runs on Jetson Nano that reads and encrypts he thermal and RGB images from the Flir Onr Pro RGB-Thermal Camera. Then the encrypted iamges will be transmitted to the database on the <a href="http://icsl.ee.columbia.edu/" target="_blank">ICSL </a> server.

For more information, please go to the <a href="http://icsl.ee.columbia.edu/FeverScreening/" target="_blank">Fever Screening</a> Offical webpage.

## Prerequisite
* hardware
Plug in a ethernet cable to Jetson Nano and power it on. Then one can either connect a external display to the Jetson Nano or ssh into it.
Power on the Flir One Pro and connect it to the Jetson Nano.

* software
run the following command in the Jetson Nano prompt\
`wget ftp://mama.indstate.edu/linux/tree/tree-1.6.0.tgz`\
`tar xzvf tree-1.6.0.tgz`\
`cd tree-1.6.0`\
`make && sudo make install`

Download this repository to local directory and install the dependencies.\
`cd ~/Fever-Edge`\
`sudo bash scripts/setup_environment.sh`

install docker:
`sudo chmod 777 scripts/setup_environment.sh`\
`sudo bash scripts/build_docker.sh flir_client_aarch64`\
`sudo bash scripts/run_container.sh delete flir_client_aarch64`\
`sudo bash scripts/run_container.sh run flir_client_aarch64`

## View the results
If all the commands above are executed successfully, send email to fever@ee.columbia.edu or go to the Fever Screeening register webpage to requist the service.
