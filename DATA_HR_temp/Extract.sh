#! /usr/bin/env bash
suffix="/FinalResults-R001/QualificationGroup/XrayCalibrationSpectrum_1/VcalCalibrationSlope_Spectrum/KeyValueDictPairs.json"
for id in 01 02 04 05 07 08 09 10 11 13 14 15 17 19 21 22 23 24 25 28 30 31 32 33 34 35 36 38 39 40 42 43 44 45 46 49 55 56 58 59 60 61
do
    ls |grep M30${id}|tail -n 1| sed "s#\.tar\.gz#${suffix}#" | xargs -n 1 cat |head -n 21|tail -n 16 |cut -c 13- |tr -d ',' >> log
done
