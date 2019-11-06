#!/usr/bin/env bash

target=${1}

if [ "${target}" = "" ]; then
    echo "Please specify a target tarball"
    exit 1
elif [ -e ${target} ]; then
    echo "Upload ${target}..."
    scp -P 23481 ${target} cern@cmspixelprod.pi.infn.it:/home/cern/dropbox
    scp ${target} pixel_dev@pcpixeltb02.cern.ch:~/MoReWeb/DATA/tar/HR
else
    echo "Please give me an input folder."
fi

