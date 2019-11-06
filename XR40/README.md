
#Control script for PHYWE X-ray expert unit - XR 4.0 


*  Is there any help/instruction to xraybox.py?

> Try `./xraybox.py`, help information should be there.  

*  Why there's no `/dev/ttyUSBx`?

>	Since the product id is not registered properly and the kernel module does not loaded automatically. After reboot, the module should be probed and settled again by
> ```sh
> sudo modprobe ftdi_sio  
> sudo sh -c 'echo 0403 a304 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id'
> ```
> Or simply `source getTTY.sh`  

* How to find which TTY is the X-ray box on?  

> Make sure the kernel module is loaded properly first.<br>
> Then find it using `dmesg | grep a304 -A20 -B20` or simply `./findTTYUSB.sh`
