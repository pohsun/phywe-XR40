#!/usr/bin/env bash
sudo modprobe ftdi_sio
sudo sh -c 'echo 0403 a304 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id'
