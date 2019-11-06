#!/usr/bin/env bash

target=${1}

if [ "${target}" = "" ]; then
    echo "Please specify a target folder"
    exit 1
elif [ -e ${target} ]; then
    ~/pxar/bin/pXar -d ~/pxar/data/${target} -T 35 -t PixAlive
    mv ${target}/pxar.log  ${target}/pxar_PixelAlive.log
    mv ${target}/pxar.root ${target}/pxar_PixelAlive.root
    ls -lhrt ${target} |tail -n 2
else
    echo "Please give me an input folder."
fi
