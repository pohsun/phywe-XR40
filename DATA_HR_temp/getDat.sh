#! /usr/bin/env bash

moduleID=$1
if [ ${moduleID} == '' ]; then
    echo "Please give me the moduleID like 'M3003'"
    exit 1
fi

targetDir=`ls ~/DATA_HR_temp/backupNew/ | grep ${moduleID}| tail -n2| head -n1`
targetTar=`ls ~/DATA_HR_temp/backupNew/ | grep ${moduleID}| tail -n1`
echo "Loading $targetDir"
cp -r ~/DATA_HR_temp/backupNew/$targetDir ~/DATA_HR_temp/
echo "Loading $targetTar"
cp -r ~/DATA_HR_temp/backupNew/$targetTar ~/DATA_HR_temp/

#targetDel=`ls ~/DATA_HR_temp/$targetDir/ | grep "Final"`
#rm -rf $targetDir/$targetDel
#echo "$targetDel has been removed!"

