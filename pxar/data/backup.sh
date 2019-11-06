#!/usr/bin/env bash

target=${1}

if [ "${target}" = "" ]; then
    echo "Please specify a target folder"
    exit 1
elif [ -e ${target} ]; then
    echo "Backup HR test result of ${target} to backup03..."
    mkdir ${target}/backup03
    cp -ar ${target}/pxar_* ${target}/XSCurveData* ${target}/backup02
    cp -ar ${target}/trimParameters35_C* ${target}/dacParameters35_C* ${target}/tbmParameters_C0* ${target}/defaultMaskFile.dat ${target}/backup03
    echo "backup03 has been completed!!"
    mkdir ${target}/original
    echo "Rename ${target} as ${target}_tmp & Store the original parameters data from CB..."
    mv ${target} ${target}_tmp
    ./getDat.sh ${target}
    cp -ar ${target}/trimParameters35_C* ${target}/dacParameters35_C* ${target}/tbmParameters_C0* ${target}/defaultMaskFile.dat ${target}_tmp/original
    mv ${target} trash/${target}_cb
    mv ${target}_tmp ${target}
    echo "Backup work has been DONE!!!"
    cp -ar ${target}/original/trimParameters35_C* ${target}/original/dacParameters35_C* ${target}/
    echo "Cold Box Data has been put in!!!"
else
    echo "Please give me an input folder."
fi
