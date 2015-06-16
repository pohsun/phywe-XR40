phywe-XR40
==========

Control script for PHYWE X-ray expert unit - XR 4.0 
```
Q:  Is there any help/instruction to xraybox.py?  
A:  Try './xraybox.py', help information should be there.  

Q:  Why there's no /dev/ttyUSBx ?  
A:	Since the product id is not registered properly and the kernel module does not loaded automatically.  
	After reboot, the module should be probed and settled again by  
        sudo modprobe ftdi_sio  
        sudo sh -c 'echo 0403 a304 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id'  
	Or simply 'source getTTY.sh'  

Q:  How to find which TTY is the X-ray box on?  
A:  Make sure the kernel module is loaded properly first.  
    Then find it using 'dmesg | grep a304 -A20 -B20'
    Or simply './findTTYUSB.sh'
```
