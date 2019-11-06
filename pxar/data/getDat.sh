#! /usr/bin/env bash

moduleID=$1
if [ ${moduleID} == '' ]; then
    echo "Please give me the moduleID like 'M3003'"
    exit 1
fi

if [ ! -e /home/pixel_dev/pxar/data/${moduleID} ]; then
    mkdir /home/pixel_dev/pxar/data/${moduleID}
else
    echo "Data for module '${module}' exists. Abort."
    exit 1
fi

targetTest=`ssh pixel_dev@pcpixelcb "ls -rtd ~/elcomandante/trunk/DATA/ThermalCycling/*/ | grep ${moduleID}_FullQualification* | tail -n1"`
echo $targetTest

targetDataDir=`ssh pixel_dev@pcpixelcb "ls -rtd ${targetTest}/*_Fulltest_p17/ | tail -n1"`
echo $targetDataDir

scp -r pixel_dev@pcpixelcb:$targetDataDir/* /home/pixel_dev/pxar/data/${moduleID}
sed -ie '1d' /home/pixel_dev/pxar/data/${moduleID}/configParameters.dat
sed -ie '1i testboardName *' /home/pixel_dev/pxar/data/${moduleID}/configParameters.dat

./CheckParameters.sh ${moduleID}
