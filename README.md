# bb_imgacquisition with Basler Camera Support

This is an edited version of bb_imgacquisition that includes support for Basler cameras. It is recommended to install the latest version of Pylon (currently 8.1.0 at the time of writing). The code also compiles with Pylon version 5 (tested with version 5.2).

## Instructions for compiling and running

These were tested on Ubuntu 20.04, 22.04, and 24.04 (see note for FLIR camera / Flycapture2 compatibility for 24.04)

### Dependencies

First install the necessary dependencies:

```bash
sudo apt install git cmake g++ libavcodec-dev libavformat-dev libavutil-dev libfmt-dev qtbase5-dev libboost-all-dev libopencv-dev libfdk-aac-dev nasm libass-dev libmp3lame-dev libopus-dev libvorbis-dev libx264-dev libx265-dev libxcb-xinput0 yasm libtool libc6 libc6-dev unzip wget libnuma1 libnuma-dev
```
### Nvidia drivers, CUDA, and Video Codec SDK for ffmpeg

Ensure that Nvidia drivers are installed (needed for hardware acceleration with ffmpeg).  CUDA is also needed. 

```bash
sudo add-apt-repository ppa:graphics-drivers/ppa
sudo apt update
sudo apt install nvidia-driver-570
sudo reboot
sudo apt install nvidia-cuda-toolkit
```

Install CUDA drivers (version 12.8), and a needed extra library for Ubuntu 24.04.  See instructions at [Nvidia - CUDA](https://developer.nvidia.com/cuda-12-4-0-download-archive?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=22.04&target_type=deb_network), or simply run the commands here.  As of May 2025, (Pytorch)[https://pytorch.org/get-started/locally/] supports up to version 12.8, so this version is recommended if you are also going to use the beesbook tracking software.

```bash
wget https://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses/libtinfo5_6.3-2ubuntu0.1_amd64.deb
sudo dpkg -i libtinfo5_6.3-2ubuntu0.1_amd64.deb # needed for Ubuntu 24
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2204/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt-get update
sudo apt install cuda-toolkit
```


The ffnvcodec is also needed.  The install instructions can also be found [on Nvidia's website](https://docs.nvidia.com/video-technologies/video-codec-sdk/11.1/ffmpeg-with-nvidia-gpu/index.html#setup) or [here](https://www.cyberciti.biz/faq/how-to-install-ffmpeg-with-nvidia-gpu-acceleration-on-linux/).

```bash
git clone https://git.videolan.org/git/ffmpeg/nv-codec-headers.git
cd nv-codec-headers && sudo make install
cd ..
```


### Install Pylon (Basler Cameras)

Download and install Pylon for Basler cameras:
Download from the [Basler website](https://www2.baslerweb.com/en/downloads/software-downloads/software-pylon-7-4-0-linux-x86-64bit-debian/) to search versions.  Tested up to version 8.1.  Download the 'Debian Installer Package' with the correct architecture for your machine (x86 or ARM).  After downloading run these commands, for example with 7.4:
```bash
tar -zxvf pylon_7_4_0_14900_linux_x86_64_debs.tar.gz 
sudo dpkg -i pylon_7.4.0.14900-deb0_amd64.deb
```

### Run the Basler USB script for system settings
This increases the USB kernel spade and file handle limit.  These commands automatically find the installed location and run the script
```bash
# Find the path to the pylon installation directory
PYLON_PATH=$(dpkg -L pylon | grep 'pylon/share/pylon/setup-usb.sh' | sed 's|/share/pylon/setup-usb.sh||')

# Run the setup script
if [ -f "$PYLON_PATH/share/pylon/setup-usb.sh" ]; then
  sudo bash "$PYLON_PATH/share/pylon/setup-usb.sh"
else
  echo "Setup script not found or not executable. Please check your installation."
fi
```

### Compile FFmpeg from Source

Download and compile FFmpeg from source with the required packages enabled. If you have FFmpeg installed through Conda or a package manager, you need to remove it first.  Cuda library locations need to be changed if they are installed other than the default locations

```bash
git clone --depth 1 https://git.ffmpeg.org/ffmpeg.git ffmpeg
cd ffmpeg
./configure --prefix=/usr/local \
            --enable-gpl \
            --enable-nonfree \
            --enable-libass \
            --enable-libfreetype \
            --enable-zlib \
            --enable-libmp3lame \
            --enable-libopus \
            --enable-libvorbis \
            --enable-libx264 \
            --enable-libx265 \
            --enable-libfdk-aac \
            --enable-cuda \
            --enable-cuvid \
            --enable-nvenc \
            --enable-libnpp \
            --extra-cflags=-I/usr/local/cuda/include \
            --extra-ldflags=-L/usr/local/cuda/lib64 \
            --extra-ldflags=-L/usr/lib/x86_64-linux-gnu \
            --extra-cflags=-I/usr/include
make -j 8
sudo make install
cd ..
```

After compiling, verify that the nvenc encoder is installed.  This should highlight the encoder.
```bash
ffmpeg -encoders | grep hevc_nvenc
```

### Clone and Build bb_imgacquisition

```bash
git clone https://github.com/BioroboticsLab/bb_imgacquisition.git
cd bb_imgacquisition
git fetch origin basler_support
git checkout basler_support

mkdir build && cd build
cmake ..
make -j 8
```

### Set permission to enable high priority runing
```
d# sudo nano /etc/security/limits.conf
# add this line:
# <YOUR_USERNAME>  -  nice    -20
```

Restart in order for changes in USB settings and permissions to take effect


## Running

### Basic usage

Run the application with a config file using the `-c` flag:

```bash
./build/bb_imgacquisition -c config.json
```

Without `-c`, it falls back to the default location `~/.config/bb_imgacquisition/config.json`. The first run generates a template config at the default path. Edit it to include camera serial numbers and other parameters. See the included `config.json` for an example.

Note!  For high resolutions, the ffmpeg encoding only supports multiples of 64 (for example, 5312x4608).  Other resolutions will lead to jumbled videos due to the encoder.

### Running multiple camera instances

Create a separate config file for each camera (e.g., `config_cam1.json`, `config_cam2.json`).  Each instance runs as its own process, which allows multiple cameras to share GPU and RAM more efficiently than a single process handling all cameras:

```bash
./build/bb_imgacquisition -c config_cam1.json &
./build/bb_imgacquisition -c config_cam2.json &
```

### Running as a systemd service (recommended)

The `setup_bbimgacq_service.sh` script creates a systemd service for a given config file.  This is the recommended way to run in production.  The service runs at high priority (`Nice=-20`), automatically restarts on failure, and enforces memory limits (3 GB soft / 4 GB hard) as a safety check.

Set up a service for each config file:

```bash
sudo ./setup_bbimgacq_service.sh config_cam1.json
sudo ./setup_bbimgacq_service.sh config_cam2.json
```

This creates services named `bb_imgacq_config_cam1.service`, `bb_imgacq_config_cam2.service`, etc.  Manage them with standard systemd commands:

```bash
sudo systemctl start bb_imgacq_config_cam1.service
systemctl status bb_imgacq_config_cam1.service
journalctl -u bb_imgacq_config_cam1.service -f
```

Logs are written to the `logs/` directory in the project root.

### Alternative: crash-restart loop script

For a simpler setup without systemd, `run_bb_imgacquisition.sh` runs the application in an infinite loop at high priority and logs crash timestamps:

```bash
./run_bb_imgacquisition.sh
```


## Optional - Install these before building to have XIMEA and/or FLIR camera support

#### Install XIMEA SDK

Dependencies:

```bash
sudo apt install libxcb-cursor0
```

Installation notes are available at [XIMEA Linux Software Package](https://www.ximea.com/support/wiki/apis/ximea_linux_software_package).

```bash
wget  https://updates.ximea.com/public/ximea_linux_sp_beta.tgz
tar xzf ximea_linux_sp_beta.tgz
cd package
bash install
cd ..
```

#### Install FlyCapture2 for FLIR camera support

Download and install FlyCapture2:

Note!  For Ubuntu 24, there are compatibility issues, because flycapture2 is no longer being updated.  So, need to add Ubuntu 22 repositories in order to install.
```bash
## Needed for Ubunt 24.04 only
sudo add-apt-repository 'deb http://archive.ubuntu.com/ubuntu jammy main universe'
sudo apt update
```

```bash
git clone https://github.com/ErnestDeiven/flycapture2
cd flycapture2/
tar -zxvf flycapture2-2.13.3.31-amd64-pkg_Ubuntu16.04.tgz
cd flycapture2-2.13.3.31-amd64/
sudo bash install_flycapture.sh
sudo systemctl restart udev  # this will restart udev, in case the error " /etc/init.d/udev: not found" comes up
cd ../../..
```
