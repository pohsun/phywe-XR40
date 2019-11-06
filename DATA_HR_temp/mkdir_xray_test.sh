#!/bin/bash

ts=`date +%F_%Hh%Mm_%s`

mkdir ${1}_XRayHRQualification_${ts}

cd ${1}_XRayHRQualification_${ts}

mkdir 000_PixelAlive_p17
mkdir 001_GainPedestal_p17
mkdir 002_RetrimHotPixels_p17
mkdir 003_RetrimHotPixels_p17
mkdir 004_HRData_50
mkdir 005_HRData_150
mkdir 006_HRScurves_10
mkdir 007_XraySpectrum_p17
mkdir 008_XraySpectrum_p17
mkdir 009_XraySpectrum_p17
mkdir 010_XraySpectrum_p17
mkdir 011_CalDelScanAndSaveDacs_10
mkdir 012_HREfficiency_50
mkdir 013_HREfficiency_75
mkdir 014_HREfficiency_100
mkdir 015_HREfficiency_125
mkdir 016_HREfficiency_150

cp -ar  /home/pixel_dev/DATA_HR_temp/configfiles .
cp -a /home/pixel_dev/local/pxar/data/${1}/configParameters.dat .
cp -a /home/pixel_dev/local/pxar/data/${1}/testParameters.dat .
for file in 0??_* ; do
    echo $file
    cp -a configParameters.dat $file
    cp -a testParameters.dat $file
done

cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_PixelAlive.*            000_PixelAlive_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_GainPedestal.*          001_GainPedestal_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/phCalibration_C*.dat         001_GainPedestal_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_retrim_0.*           002_RetrimHotPixels_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/defaultMaskFile.dat          002_RetrimHotPixels_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_retrim_50.*          003_RetrimHotPixels_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/defaultMaskFile.dat          003_RetrimHotPixels_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_Ag_current_20.*      004_HRData_50/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_Ag_current_50.*      005_HRData_150/
cp -a /home/pixel_dev/local/pxar/data/${1}/XSCurveData_current_10/*.dat 006_HRScurves_10/
cp -a /home/pixel_dev/local/pxar/data/${1}/SCurveData_C*.dat            006_HRScurves_10/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_scurve.*             006_HRScurves_10/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_Zn.* 007_XraySpectrum_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_Mo.* 008_XraySpectrum_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_Ag.* 009_XraySpectrum_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_Sn.* 010_XraySpectrum_p17/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_current_10.*   011_CalDelScanAndSaveDacs_10/
cp -a /home/pixel_dev/local/pxar/data/${1}/dacParameters35_C*.dat 011_CalDelScanAndSaveDacs_10/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_current_10.*   012_HREfficiency_50/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_current_20.*   013_HREfficiency_75/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_current_30.*   014_HREfficiency_100/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_current_40.*   015_HREfficiency_125/
cp -a /home/pixel_dev/local/pxar/data/${1}/pxar_HR_current_50.*   016_HREfficiency_150/

tar -czf ../${1}_XRayHRQualification_${ts}.tar.gz ../${1}_XRayHRQualification_${ts}
