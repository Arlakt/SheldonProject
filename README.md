# Sheldon Project
Sheldon project makes a [Parrot A.R. Drone 2.0](http://ardrone2.parrot.com/) follow a beacon.  
It takes place within the [Critical Embedded Systems course](http://gei.insa-toulouse.fr/fr/formation_initiale/drones.html) at [INSA Toulouse](http://www.insa-toulouse.fr/) engineering school.

Website: https://sites.google.com/site/projetsecinsa/projets-2015-2016/projct-sheldon

The project features:
- Embedded software to be run on the drone
- Embedded board and sensors to receive the beacon's signal
- The beacon, a device emitting ultrasonic waves

## How to use the embedded software
1. Install the GNU C compiler for ARM: gcc-arm-linux-gnueabi  
Packages are available for most Linux distributions
2. Build the "CDC-ACM" kernel module and upload it to the drone  
see https://sites.google.com/site/projetsecinsa/documents-techniques/outils-et-methodes/cross-compiler-pour
3. Make sure that a `/dev/ttyACM*` device is created when you plug the receiver board
4. Clone the repository and build the program:
```
git clone https://github.com/Arlakt/SheldonProject.git
cd SheldonProject/embedded-sw/
make
```
5. Upload it to the drone and start a telnet session to run the program:
`./upload.sh`
```
# insmod /data/video/modules/cdc-acm.ko
# chmod +x /data/video/main.elf
# /data/video/main.elf
```

## Authors
[Yorrick Barnay](https://github.com/Elskou), [Julien Cassette](https://github.com/jcassette), [Raphaël Roy](https://github.com/raphael-roy), [Romain Taprest](https://github.com/Arlakt)
