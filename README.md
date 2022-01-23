# MULTI-PERSON FEVER SCREENING

## About
This repository is the program for the fever screening edge device. The edge device consists of a <a href="https://www.amazon.com/FLIR-Thermal-Imaging-Microfiber-Cleaning/dp/B07NS5WNRD/" target="_blank">Flir One Pro</a> and a <a href="https://usedipaq.com/products/nvidia-jetson-nano-developer-kit-2gb" target="_blank">Jetson Nano </a>.

The program runs on Jetson Nano that reads and encrypts he thermal and RGB images from the Flir Onr Pro RGB-Thermal Camera. Then the encrypted iamges will be transmitted to the database on the <a href="http://icsl.ee.columbia.edu/" target="_blank">ICSL </a> server.

For more information, please go to the <a href="http://icsl.ee.columbia.edu/FeverScreening/" target="_blank">Fever Screening</a> Offical webpage.

## How to join
Send an email to kh3119@columbia.edu to request a fever screening service. We will send you an json configuration file along with the web client address. \
In the email please include the number of hardware (Flir One Pro and Jetson Nano) you have.
Alternatively, you can fill the <a href="https://forms.gle/HrrUpZk58gDJknvZ8" target="_blank">form</a>.


## Setup the edge
* hardware
Plug in a ethernet cable to Jetson Nano and power it on. Then one can either connect a external display to the Jetson Nano or ssh into it.
Power on the Flir One Pro and connect it to the Jetson Nano.

* software
run the following command in the Jetson Nano prompt\
`wget ftp://mama.indstate.edu/linux/tree/tree-1.6.0.tgz`\
`tar xzvf tree-1.6.0.tgz`\
`cd tree-1.6.0`\
You need to enter your password for the following command
`make && sudo make install`

Download this repository to local directory and install the dependencies.\
`git clone https://github.com/Columbia-ICSL/FeverScreening-Edge.git`\
Then place the launch.json file you received from email under the FeverScreening-Edge/config directory.\
'cd FeverScreening-Edge'\
`sudo bash scripts/setup_environment.sh`

https://github.com/bazelbuild/bazelisk/releases bazelisk-linux-arm64

install docker:
`sudo chmod 777 scripts/setup_environment.sh`\
`sudo bash scripts/build_docker.sh flir_client_aarch64`\
`sudo bash scripts/run_container.sh delete flir_client_aarch64`\
`sudo bash scripts/run_container.sh run flir_client_aarch64`

## View the results
Results will be shown on a web client at icsl-lambda.ee.columbia.edu:{PORT}. The PORT is the port number we sent to you in the email.
