#!/usr/bin/env bash

target=${1}

if [ "${target}" = "" ]; then
    echo "Please specify a target folder"
    exit 1
elif [ -e ${target} ]; then
    cd ${target}_doNOTkill/
    cp -ar trimParameters35_C* dacParameters35_C* pxar_HR_* XSCurveData_current_10/ pxar_PixelAlive.* defaultMaskFile.dat ../${target}/
    cd ..
    ls -lhrt ${target}
else
    echo "Please give me an input folder."
fi
