#!/bin/sh

Dir="/home/pixel_dev/pxar/data/$1/"
target="testParameters.dat"
tmp="tmp.dat"
log="whichLineLog"

cp ${Dir}${target} ${tmp}

grep "trimhotpixels" ${tmp}
if [ $? != 0 ];then
    grep -nA24 HighRate ${tmp} | grep PIX | awk -F "-" '{printf("%s\n",$1)}' > ${log}
    cat ${log} | awk '{printf("-i \"%da maskuntrimmable     checkbox(1)\" tmp.dat\n",$1)}' | xargs -n 3 sed
    cat ${log} | awk '{printf("-i \"%da savetrimbits        checkbox(1)\" tmp.dat\n",$1)}' | xargs -n 3 sed
    cat ${log} | awk '{printf("-i \"%da runsecondsHotPixels 1\" tmp.dat\n",$1)}' | xargs -n 3 sed
    cat ${log} | awk '{printf("-i \"%da trimhotpixelthr     200\" tmp.dat\n",$1)}' | xargs -n 3 sed
    cat ${log} | awk '{printf("-i \"%da trimhotpixels       button\" tmp.dat\n",$1)}' | xargs -n 3 sed
fi

grep -nA24 HighRate ${tmp} | grep Dacs | awk -F "-" '{printf("%s\n",$1)}' > ${log}
cat ${log} | awk '{printf("-i \"%dc Dacs/Step           1\" tmp.dat\n",$1)}' | xargs -n 3 sed

mv ${tmp} ${Dir}${target}
rm ${log}


