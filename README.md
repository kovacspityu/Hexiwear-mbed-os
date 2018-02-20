This project is aimed mainly at learning a variety of tools for programming and maintaining a repository.    
It is intended as a personal project, not ready for wide use, and as of right now doesn't accept contributors.


The final aim is to deploy a complete software package for the [Hexiwear](hexiwear.com) hardware,    
a gadget produced by [MikroElektronika](mikroe.com) with a variety of sensors in a    
compact package that requires no soldering or tinkering.


The project is based on the [mbed OS](mbed.com), a complete os for arm-based hardware.


To build the project you will need the hexiwear docking station or equivalent programmer.    
Software wise, you'll need mbed CLI or make use of the mbed online compiler.    
The most updated instructions will always be available on mbed documentation site, os.mbed.com/docs/    

As of right now, these are the necessary steps:
* Install mbed CLI through pip, calling
` pip install mbed `
* Move to the root directory of the project, and create a new project with
`mbed new .`
This will take a while, as it needs to download the entirety of the mbed library
* You will want to setup the default target and toolchain to speed up your process with
`mbed target hexiwear`
`mbed toolchain GCC_ARM`
You might need to modify the mbed_setting.py file to reflect the location of your compiler.
* At this point you should be ready to compile the project with
`mbed compile`
The binary file will then appear in the BUILD directory, or you could also add the -f option
to the compile command for mbed CLI to attempt to flash the binary to the connected hexiwear.