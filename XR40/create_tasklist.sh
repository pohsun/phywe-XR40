#/bin/sh -f 

export MNAME1=$1 # the outer adapter
export MNAME2=$2 # the inner adapter
dtbId4OuterAda=DTB_WV7PE4 # The inner DTB/outer adapter, #100, FW:4.0
dtbId4InnerAda=DTB_WV8A2R # The outer DTB/inner adapter, #114, FW:4.0

function addMNAME {
    if [[ $MNAME2 != "" ]]; then
        sed -i "1s/.*/testboardName ${dtbId4InnerAda}/" ~/pxar/data/${MNAME2}/configParameters.dat
        mv tasks tasks.single
        mv tasks_HR tasks_HR.single
        #mv tasks_timing tasks_timing.single
        #mv tasks_alive tasks_alive.single
        awk '$0 !~ MNAME1{printf("%s\n",$0)}$0 ~ MNAME1{printf("%s\n",$0);gsub(MNAME1,MNAME2,$0);printf("%s\n",$0)}' MNAME1=${MNAME1} MNAME2=${MNAME2} tasks.single > tasks
        awk '$0 !~ MNAME1{printf("%s\n",$0)}$0 ~ MNAME1{printf("%s\n",$0);gsub(MNAME1,MNAME2,$0);printf("%s\n",$0)}' MNAME1=${MNAME1} MNAME2=${MNAME2} tasks_HR.single > tasks_HR
        #awk '$0 !~ MNAME1{printf("%s\n",$0)}$0 ~ MNAME1{printf("%s\n",$0);gsub(MNAME1,MNAME2,$0);printf("%s\n",$0)}' MNAME1=${MNAME1} MNAME2=${MNAME2} tasks_timing.single > tasks_timing
        #awk '$0 !~ MNAME1{printf("%s\n",$0)}$0 ~ MNAME1{printf("%s\n",$0);gsub(MNAME1,MNAME2,$0);printf("%s\n",$0)}' MNAME1=${MNAME1} MNAME2=${MNAME2} tasks_alive.single > tasks_alive
        rm tasks.single
        rm tasks_HR.single
        #rm tasks_timing.single
        #rm tasks_alive.single
    fi
}


usb=`./findTTYUSB.sh | grep PHYWE_X-Ray_09057.99 | awk '{print $1}'`
sed -i "1s/.*/testboardName ${dtbId4OuterAda}/" ~/pxar/data/${MNAME1}/configParameters.dat


#cat > tasks_alive <<EOF
## ${usb}
#
#setGonio 900 900 1 1 3
#external sleep 3
#startRun 1
#
#external sleep 3
#parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t PixelAlive
#parallel wait
#external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_PixelAlive.root
#external mv ~/pxar/data/${MNAME1}/pxar.log ~/pxar/data/${MNAME1}/pxar_PixelAlive.log
#external sleep 3
#EOF


cat > tasks_timing <<EOF
# ${usb}

setGonio 900 900 1 1 3
external sleep 3
startRun 1
external sleep 3

setXray 0
#parallel cat HighRate_timing | ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -v DEBUG
external ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t cmd:timing
external mv `pwd`/pxar_timing.log  ~/pxar/data/${MNAME1}/pxar_HR_timing_current_0.log

setCurrent 50
external sleep 3
setXray 1
external sleep 10
#parallel cat HighRate_timing | ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -v DEBUG
external ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t cmd:timing
external mv `pwd`/pxar_timing.log  ~/pxar/data/${MNAME1}/pxar_HR_timing_current_50.log
setXray 0

external grep "selecting" ~/pxar/data/${MNAME1}/pxar_HR_timing_current_0.log | tail -n 1 | cut -d ' ' -f 2-7 | awk 'NR<6{printf("ibase=10;obase=2;%d\nibase=10;obase=2;%d\nibase=10;obase=2;%d\nibase=10;obase=2;%d\nibase=10;obase=2;%d\nibase=10;obase=2;%d\n0\n0",\$5,\$4,\$3,\$3,\$1,\$2)}' | xargs -L 1| bc | awk 'BEGIN{printf("ibase=2;obase=1010;")}NR>2 && NR<7{printf("%03d",\$1)}NR==4{printf("\nibase=2;obase=1010;")}NR<3 || NR>6{printf("%d",\$1)}END{printf("\n")}' | xargs -L 1 | bc | awk '{printf("0x%02X\n",\$1)}' | awk '{if(NR==1)printf("basea   %s\n",\$1)}{if(NR==2)printf("basee   %s\n",\$1)}'| tee -a ~/pxar/data/${MNAME1}/pxar_tbmParameters.log 
external grep -n "basea" ~/pxar/data/${MNAME1}/pxar_tbmParameters.log | tail -n1 | awk '{printf("-i \"%di tbmParameters with current=0\" /home/pixel_dev/pxar/data/${MNAME1}/pxar_tbmParameters.log\n",\$1)}'| xargs -n 3 sed

external grep "selecting" ~/pxar/data/${MNAME1}/pxar_HR_timing_current_50.log | tail -n 1 | cut -d ' ' -f 2-7 | awk 'NR<6{printf("ibase=10;obase=2;%d\nibase=10;obase=2;%d\nibase=10;obase=2;%d\nibase=10;obase=2;%d\nibase=10;obase=2;%d\nibase=10;obase=2;%d\n0\n0",\$5,\$4,\$3,\$3,\$1,\$2)}' | xargs -L 1| bc | awk 'BEGIN{printf("ibase=2;obase=1010;")}NR>2 && NR<7{printf("%03d",\$1)}NR==4{printf("\nibase=2;obase=1010;")}NR<3 || NR>6{printf("%d",\$1)}END{printf("\n")}' | xargs -L 1 | bc | awk '{printf("0x%02X\n",\$1)}'| awk '{if(NR==1)printf("basea   %s\n",\$1)}{if(NR==2)printf("basee   %s\n",\$1)}'| tee -a ~/pxar/data/${MNAME1}/pxar_tbmParameters.log 
external grep -n "basea" ~/pxar/data/${MNAME1}/pxar_tbmParameters.log | tail -n1 | awk '{printf("-i \"%di tbmParameters with current=50\" /home/pixel_dev/pxar/data/${MNAME1}/pxar_tbmParameters.log\n",\$1)}'| xargs -n 3 sed

external sleep 3
#resetAll
EOF


cat > tasks <<EOF
# ${usb}

external sleep 3

setCurrent 100
setGonio 0 0 1 1 3
external sleep 3
startRun 1

external sleep 3
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t GainPedestal
parallel wait
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_GainPedestal.root
external mv ~/pxar/data/${MNAME1}/pxar.log ~/pxar/data/${MNAME1}/pxar_GainPedestal.log
external cp ~/pxar/data/${MNAME1}/defaultMaskFile.dat ~/pxar/data/${MNAME1}/defaultMaskFile.dat.old
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t Xray:maskHotPixels -p "savemaskfile=1"
parallel wait
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_maskHotPixels.root
external cat ~/pxar/data/${MNAME1}/defaultMaskFile.dat.old >> ~/pxar/data/${MNAME1}/defaultMaskFile.dat
external cat ~/pxar/data/${MNAME1}/defaultMaskFile.dat | sort -n -k2 -k3 -k4 | uniq | tee ~/pxar/data/${MNAME1}/defaultMaskFile.dat
#external vim ~/pxar/data/${MNAME1}/defaultMaskFile.dat
external sleep 3

# steps for taking xray data
setGonio 600 600 1 2 3
external sleep 3
startRun 1
external sleep 3
setXray 1
external sleep 5
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t Xray:phrun -p "source=Mo;runseconds=100" #240 #100
parallel wait
setXray 0
parallel mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_Mo.root
parallel mv ~/pxar/data/${MNAME1}/pxar.log ~/pxar/data/${MNAME1}/pxar_Mo.log
external sleep 3

# Draw Xray spectrum and view 
external root.exe ~/pxar/macros/Draw_module_map.C\(\"~/pxar/data/${MNAME1}/pxar_Mo.root\",\"Xray\",\"Mo\",0\)

#--------------------------------------------
setGonio 1500 1500 1 2 3
external sleep 3
startRun 1
external sleep 3
setXray 1
external sleep 5
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t Xray:phrun -p "source=Zn;runseconds=100" #240 #100
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_Zn.root
external mv ~/pxar/data/${MNAME1}/pxar.log ~/pxar/data/${MNAME1}/pxar_Zn.log
external sleep 3
# Draw Xray spectrum and view 
external root.exe ~/pxar/macros/Draw_module_map.C\(\"~/pxar/data/${MNAME1}/pxar_Zn.root\",\"Xray\",\"Zn\",0\)
#--------------------------------------------
setGonio 2400 2400 1 2 3
external sleep 3
startRun 1
external sleep 3
setXray 1
external sleep 5
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t Xray:phrun -p "source=Sn;runseconds=200" #240 #600
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_Sn.root
external mv ~/pxar/data/${MNAME1}/pxar.log ~/pxar/data/${MNAME1}/pxar_Sn.log
external sleep 3
# Draw Xray spectrum and view 
external root.exe ~/pxar/macros/Draw_module_map.C\(\"~/pxar/data/${MNAME1}/pxar_Sn.root\",\"Xray\",\"Sn\",0\)
#--------------------------------------------
setGonio 3300 3300 1 2 3
external sleep 3
startRun 1
external sleep 3
setXray 1
external sleep 5
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t Xray:phrun -p "source=Ag;runseconds=100" #240 #400
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_Ag.root
external mv ~/pxar/data/${MNAME1}/pxar.log ~/pxar/data/${MNAME1}/pxar_Ag.log
external sleep 3
external root.exe ~/pxar/macros/Draw_module_map.C\(\"~/pxar/data/${MNAME1}/pxar_Ag.root\",\"Xray\",\"Ag\",0\)
resetAll
EOF


cat > tasks_HR <<EOF
#${usb}

external echo "\n\t!!! Please make sure door is locked and control panel is at monitoring stage. !!! \n"
external echo "\n\t!!! ---------------------------------------------------------- \n"
external echo "\n\t!!! Have you masked out the noisy pixels? !!! \n"
external echo "\n \n"
external sleep 3

setCurrent 10
setGonio 900 900 1 1 3
external sleep 3
startRun 1

# steps for pixelAlive test
external sleep 3
external mv ~/pxar/data/${MNAME1}/defaultMaskFile.dat ~/pxar/data/${MNAME1}/defaultMaskFile.dat.old
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t PixelAlive
parallel wait
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_PixelAlive.root
external mv ~/pxar/data/${MNAME1}/pxar.log ~/pxar/data/${MNAME1}/pxar_PixelAlive.log
external sleep 5 

# steps for re-trim test
setCurrent 50
setXray 1
external sleep 10
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t HighRate:trimhotpixels -p "trimhotpixelthr=200;runsecondshotpixels=1;savetrimbits=1;maskuntrimmable=1" -v DEBUG
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_retrim_50.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_retrim_50.log
#external vim ~/pxar/data/${MNAME1}/defaultMaskFile.dat
external sleep 3

setCurrent 0
setXray 0
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t HighRate:trimhotpixels -p "trimhotpixelthr=10;runsecondshotpixels=5;savetrimbits=1;maskuntrimmable=1" -v DEBUG
parallel wait
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_retrim_0.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_retrim_0.log
#external vim ~/pxar/data/${MNAME1}/defaultMaskFile.dat
external sleep 3


# steps for takin HR hitmap
setCurrent 50
setXray 1
external sleep 10
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t Xray:phrun -p "source=Ag;runseconds=100"
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_Ag_current_50.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_Ag_current_50.log
external root.exe ~/pxar/macros/Draw_module_map.C\(\"~/pxar/data/${MNAME1}/pxar_HR_Ag_current_50.root\",\"Xray\",\"Ag\",0\)
external sleep 3

setCurrent 20
setXray 1
external sleep 10
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t Xray:phrun -p "source=Ag;runseconds=100"
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_Ag_current_20.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_Ag_current_20.log
external root.exe ~/pxar/macros/Draw_module_map.C\(\"~/pxar/data/${MNAME1}/pxar_HR_Ag_current_20.root\",\"Xray\",\"Ag\",0\)
external sleep 3


# steps for taking HR data
setCurrent 10
setXray 1
external sleep 10
parallel ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -t HighRate:xnoisemaps -p "ntrig=25" -v DEBUG
parallel wait
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_scurve.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_scurve.log
external mkdir ~/pxar/data/${MNAME1}/XSCurveData_current_10
external mv ~/pxar/data/${MNAME1}/XSCurve*.dat  ~/pxar/data/${MNAME1}/XSCurveData_current_10
external sleep 3
parallel cat HighRate_tests_low_flux | ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -p "triggerdelay=10;trgfrequency(khz)=30;ntrig=50" -v DEBUG
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_current_10.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_current_10.log
external sleep 3

setCurrent 20
setXray 1
external sleep 10
parallel cat HighRate_tests | ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -p "triggerdelay=10;trgfrequency(khz)=30;ntrig=50" -v DEBUG
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_current_20.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_current_20.log
external sleep 3

setCurrent 30
setXray 1
external sleep 10
parallel cat HighRate_tests | ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -p "triggerdelay=10;trgfrequency(khz)=30;ntrig=50" -v DEBUG
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_current_30.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_current_30.log
external sleep 3

setCurrent 40
setXray 1
external sleep 10
parallel cat HighRate_tests | ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -p "triggerdelay=40;trgfrequency(khz)=30;ntrig=50" -v DEBUG
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_current_40.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_current_40.log
external sleep 3

setCurrent 50
setXray 1
external sleep 10
parallel cat HighRate_tests | ~/pxar/bin/pXar -d ~/pxar/data/${MNAME1} -T 35 -p "triggerdelay=40;trgfrequency(khz)=30;ntrig=50" -v DEBUG
parallel wait
setXray 0
external mv ~/pxar/data/${MNAME1}/pxar.root ~/pxar/data/${MNAME1}/pxar_HR_current_50.root
external mv ~/pxar/data/${MNAME1}/pxar.log  ~/pxar/data/${MNAME1}/pxar_HR_current_50.log
external sleep 3

resetAll

EOF

addMNAME

