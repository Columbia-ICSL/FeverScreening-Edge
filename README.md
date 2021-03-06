# MULTI-PERSON FEVER SCREENING

## About
This repository is the program for the fever screening edge device. The edge device consists of a <a href="https://www.amazon.com/Thermal-Imaging-Android-USB-C-Resolution/dp/B0716ZGL7C" target="_blank">Flir One Pro</a> and a Jetson Nano(<a href="https://www.amazon.com/NVIDIA-Jetson-Nano-Developer-945-13541-0000-000/dp/B08J157LHH/ref=pd_lpo_147_t_0/146-6755111-0987264?_encoding=UTF8&pd_rd_i=B08J157LHH&pd_rd_r=46d84ae7-03c8-43ef-9e86-7663a167f108&pd_rd_w=V8Qia&pd_rd_wg=Y1BqF&pf_rd_p=7b36d496-f366-4631-94d3-61b87b52511b&pf_rd_r=TS8FB5WP72FETGNKJ7ET&psc=1&refRID=TS8FB5WP72FETGNKJ7ET" target="_blank">2GB</a> or <a href="https://www.amazon.com/NVIDIA-Jetson-Nano-Developer-945-13450-0000-100/dp/B084DSDDLT" target="_blank">4GB</a>).

The program runs on Jetson Nano that reads and encrypts he thermal and RGB images from the Flir Onr Pro RGB-Thermal Camera. Then the encrypted iamges will be transmitted to the database on the <a href="http://icsl.ee.columbia.edu/" target="_blank">ICSL </a> server.

For more information, please go to the <a href="http://icsl.ee.columbia.edu/FeverScreening/" target="_blank">Fever Screening</a> Offical webpage.

## How to join
Send an email to kh3119@columbia.edu to request a fever screening service. We will send you an json configuration file along with the web client address. \
Alternatively, you can fill the <a href="https://forms.gle/HrrUpZk58gDJknvZ8" target="_blank">form</a>.


## Setup the edge
* Get start on Jetson nano. This step may take about 20 minutes. 

  * <a href="https://developer.nvidia.com/embedded/learn/get-started-jetson-nano-2gb-devkit" target="_blank">Preparation for Jetson Nano 2GB Developer Kit</a>. 
    1. Download images for <a href="https://developer.nvidia.com/jetson-nano-2gb-sd-card-image">Jetson Nano 2GB Developer Kit SD Card Image</a>.
    2. Write the image to your microSD card according to your operating system. 

  * <a href="https://developer.nvidia.com/embedded/learn/getting-started-jetson" target="_blank">Other versions of Jetson Nano can be found here</a>.

* hardware
  * You need a male usb to female usb-c cable adapter(<a href="https://www.amazon.com/5inch-USB-3-1-Adapter-Pack/dp/B087R1GZ1Q/" target="_blank">Link</a>), ethenet cable or network interface card, a display, mouse and keyboard.
 
Plug in a ethernet cable to Jetson Nano, and connect mouse, keyboard and monitor to it, then power it on. You will be asked to setup the system configuration, you can select default options. 

* software
run the following command in the Jetson Nano prompt\
`wget ftp://mama.indstate.edu/linux/tree/tree-1.6.0.tgz`\
`tar xzvf tree-1.6.0.tgz`\
`cd tree-1.6.0`\
You need to enter your password for the following command
`make && sudo make install`\
`cd ..`

Download this repository to local directory and install the dependencies.\
`git clone https://github.com/Columbia-ICSL/FeverScreening-Edge.git`\
:heavy_exclamation_mark:  Add `machine_id` and `server_port` in launch.json file you received from email under the FeverScreening-Edge/config directory.\
`cd FeverScreening-Edge`

`sudo bash scripts/setup_environment.sh`

Download bazel at: https://github.com/bazelbuild/bazelisk/releases bazelisk-linux-arm64\
Or by command `sudo wget https://github.com/bazelbuild/bazelisk/releases/download/v1.11.0/bazelisk-linux-arm64 -O /usr/local/bin/bazel`\
`sudo chmod 777 /usr/local/bin/bazel`

`sudo chmod 777 scripts/setup_environment.sh`

`sudo bash scripts/build_docker.sh flir_client_aarch64`\
This command may take about half hour on 2GB verison of Jetson Nano.

Then connect flir one to Jetson nano and make sure Flir one is giving a green light on side. 
`sudo bash scripts/run_container.sh run flir_client_aarch64`\
Note: if on the terminal it is saying wait for starting, then the Flir one is not running. \
Try `sudo docker stop flir_client_aarch64` or `sudo bash scripts/run_container.sh delete flir_client_aarch64`\
and run again with command `sudo bash scripts/run_container.sh run flir_client_aarch64`

## View the results
Results will be shown on a web client at icsl-lambda.ee.columbia.edu:{PORT}. The PORT is the port number we sent to you in the email.

