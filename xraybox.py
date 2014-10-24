#!/usr/bin/env python
# vim: set sw=4 sts=4 fdm=marker et:
#-*- coding: utf-8 -*-

import sys
import os
import os.path
import readline
import subprocess
import inspect
import serial
import time
import struct

readline.parse_and_bind('tab: complete')
readline.parse_and_bind('set editing-mode vi')

### Class
class Xraybox:
    """A class for phywe XR-40 x-ray box"""
    _modeDict = {'dataReqB': 0,
        'dataReqA'    : 1,
        'ansReqB'     : 2,
        'write'       : 3,
        'read'        : 4,
        'ansWriteOk'  : 5,
        'ansWriteNok' : 6,
        'valDevPar'   : 7,
        'cmdOk'       : 8,
        'cmdError'    : 9,
        'xray'        : 10,
        'light'       : 11,
        'speaker'     : 12,
        'gonioCali'   : 13,
        'xrStageZero' : 14,
        'xrPosZero'   : 15,
        'start'       : 16,
        'stop'        : 17,
        'answer'      : 18,
        'error'       : 19,
        'null'        : 20}
    _modeData = (('dataReqB'    , 0x01 , 0 , 0) ,# modeName, modeIndex, modeHavePara, _modeDataLength
        ('dataReqA'    , 0x08 , 0 , 0) ,
        ('ansReqB'     , 0x09 , 0 , 0) ,
        ('write'       , 0x11 , 1 , 0) ,
        ('read'        , 0x12 , 1 , 0) ,
        ('ansWriteOk'  , 0x18 , 0 , 0) ,
        ('ansWriteNok' , 0x19 , 0 , 0) ,
        ('valDevPar'   , 0x1a , 0 , 0) ,
        ('cmdOk'       , 0x1b , 0 , 0) ,
        ('cmdError'    , 0x1f , 0 , 0) ,
        ('xray'        , 0x2a , 0 , 1) ,
        ('light'       , 0x2b , 0 , 1) ,
        ('speaker'     , 0x2c , 0 , 1) ,
        ('gonioCali'   , 0x2e , 0 , 1) ,
        ('xrStageZero' , 0x2f , 0 , 0) ,
        ('xrPosZero'   , 0x30 , 0 , 0) ,
        ('start'       , 0x31 , 0 , 0) ,
        ('stop'        , 0x32 , 0 , 0) ,
        ('answer'      , 0x38 , 0 , 0) ,
        ('error'       , 0xff , 0 , 0) ,
        ('null'        , 0x00 , 0 , 0) )
    _deviceParaDict = {'hwVersion':0,
        'fwVersion'        :1,
        'devClass'         :2,
        'highVoltage'      :3,
        'current'          :4,
        'doorStatus'       :5,
        'tubeStatus'       :6,
        'changeStatus'     :7,
        'tubeIO'           :8,
        'lightIO'          :9,
        'speakerIO'        :10,
        'gonioStartAngX10' :11,
        'gonioStopAngX10'  :12,
        'gonioMaxStopAng'  :13,
        'gonioIncrementX10':14,
        'gonioPosition'    :15,
        'gmVoltage'        :16,
        'gmGateTime'       :17,
        'devOperHrs'       :18,
        'tubeOperHrs'      :19,
        'date'             :20,
        'time'             :21,
        'gonioMode'        :22,
        'snDate'           :23,
        'sn'               :24,
        'gonioStatus'      :25,
        'fixAngle'         :26,
        'timerMode'        :27,
        'startDelay'       :28,
        'timerVal'         :29,
        'crystalType'      :30,
        'xrStageStatus'    :31,
        'xrStateStopVal'   :32,
        'increment'        :33,
        'waitTimeX10'      :34,
        'xrStageMode'      :35,
        'null'             :36}
    _deviceParaData = (('hwVersion'         , 0x00 , 1) ,
        ('fwVersion'         , 0x01 , 2) ,
        ('devClass'          , 0x02 , 1) ,
        ('highVoltage'       , 0x03 , 2) ,
        ('current'           , 0x04 , 1) ,
        ('doorStatus'        , 0x05 , 1) ,
        ('tubeStatus'        , 0x06 , 1) ,
        ('changeStatus'      , 0x07 , 4) ,
        ('tubeIO'            , 0x08 , 1) ,
        ('lightIO'           , 0x09 , 1) ,
        ('speakerIO'         , 0x0a , 1) ,
        ('gonioStartAngX10'  , 0x0b , 2) ,
        ('gonioStopAngX10'   , 0x0c , 2) ,
        ('gonioMaxStopAng'   , 0x0d , 1) ,
        ('gonioIncrementX10' , 0x0e , 2) ,
        ('gonioPosition'     , 0x10 , 1) ,
        ('gmVoltage'         , 0x11 , 2) ,
        ('gmGateTime'        , 0x12 , 2) ,
        ('devOperHrs'        , 0x13 , 4) ,
        ('tubeOperHrs'       , 0x14 , 4) ,
        ('date'              , 0x15 , 4) ,
        ('time'              , 0x16 , 4) ,
        ('gonioMode'         , 0x17 , 1) ,
        ('snDate'            , 0x18 , 2) ,
        ('sn'                , 0x19 , 4) ,
        ('gonioStatus'       , 0x1a , 1) ,
        ('fixAngle'          , 0x1b , 2) ,
        ('timerMode'         , 0x1c , 1) ,
        ('startDelay'        , 0x1d , 2) ,
        ('timerVal'          , 0x1e , 2) ,
        ('crystalType'       , 0x1f , 1) ,
        ('xrStageStatus'     , 0x22 , 1) ,
        ('xrStateStopVal'    , 0x24 , 2) ,
        ('increment'         , 0x25 , 1) ,
        ('waitTimeX10'       , 0x26 , 2) ,
        ('xrStageMode'       , 0x27 , 1) ,
        ('null'              , 0x00 , 0) )

    def __init__(self, iPort="/dev/ttyUSB0", commtime=0.1):
        """Initiate the XR-40 box"""
        if os.path.exists(iPort):
            self._usbserial = serial.Serial(
                port=iPort,
                baudrate=921600,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                bytesize=serial.FIVEBITS,
                timeout=1)
        else:
            print """ERROR\t\t: '{0}' doesn't exist!""".format(iPort)
            sys.exit(1)
        self.commtime = commtime
        self._measureStatus = 0

    def getHWversion(self):
        """Hardware version"""
        return int(self._getDevStatus(0)[0],16)

    def getFWversion(self):
        """Firmware version"""
        oBytes = self._getDevStatus(1)
        return """{0}.{1}""".format(int(oBytes[0],16),int(oBytes[1],16))

    def getHV(self): #unit:100V
        """High Voltage [0.1 kV]"""
        return self._hex2dec(self._getDevStatus(3))

    def getCurrent(self):
        """Current [10 mA]"""
        return int(self._getDevStatus(4)[0],16)

    def getDoor(self):
        """Status of door (0:unlocked, 1:locked)"""
        return int(self._getDevStatus(5)[0],16)

    def getLight(self):
        """Status of light (0:off, 1:on)"""
        return int(self._getDevStatus(9)[0],16)

    def getSpeaker(self):
        """Status of speaker (0:off, 1:on)"""
        return int(self._getDevStatus(10)[0],16)

    def getGonioStatus(self):
        """startAngle[0.1 deg], stopAngle[0.1 deg], stepSize[0.1 deg], Mode(1:fixCrystal, 2:fixDetector)"""
        oBytesStart = self._getDevStatus(11)
        oBytesStop  = self._getDevStatus(12)
        oBytesSteps = self._getDevStatus(14)
        oBytesMode  = self._getDevStatus(22)
        #oBytesPos   = self._getDevStatus(15)
        return [self._hex2dec(oBytesStart),self._hex2dec(oBytesStop),self._hex2dec(oBytesSteps),self._hex2dec(oBytesMode)]

    def getGMVoltage(self):
        """GM Voltage [0.1 kV]"""
        oBytes = self._getDevStatus(16)
        return self._hex2dec(oBytes) #0.1 kV

    def getGMGate(self):
        """GM Gate time [0.1 second] - Time interval between steps"""
        oBytes = self._getDevStatus(17)
        return self._hex2dec(oBytes) #0.1s

    def getDate(self):
        """Date is YYYY/MM/DD"""
        oBytes = self._getDevStatus(20)
        return """{0}/{1}/{2}""".format(int(oBytes[0],16)*256+int(oBytes[1],16),int(oBytes[2],16),int(oBytes[3],16))

    def getTime(self):
        """Time is hh:mm:ss"""
        oBytes = self._getDevStatus(21)
        return """{0}:{1}:{2}""".format(int(oBytes[0],16),int(oBytes[1],16),int(oBytes[2],16))

    def setLight(self, iStatus=-1):
        """setLight 0(off)/1(on)/-1(switch)"""
        iniStatus = self.getLight()
        if iStatus == -1 or iniStatus != iStatus:
            self._send(self._interpret(11,(iniStatus+1)%2))
            if not self._isCmdOk("""ERROR\t\t: Got some error in setLight, status unchanged."""):
                sys.exit(1)

    def setSpeaker(self, iStatus=-1):
        """setSpeaker 0(off)/1(on)/-1(switch)"""
        iniStatus = self.getSpeaker()
        if iStatus == -1 or iniStatus != iStatus:
            self._send(self._interpret(12,(iniStatus+1)%2))
            if not self._isCmdOk("""ERROR\t\t: Got some error in setSpeaker, status unchenged."""):
                sys.exit(1)

    def setXray(self, iStatus):
        """setXray 0(off)/1(on)/-1(switch)"""
        if self.getDoor() != 1:
            print """WARNING\t\t: Unable to switch Xray on with door UNLOCKED."""
            return
        else:
            self._send(self._interpret(10,iStatus))
            if not self._isCmdOk("""WARNING\t\t: Xray could be at the wanteds already."""):
                self._send(self._interpret(10,(iStatus+1)%2))
                self._read()
                self._send(self._interpret(10,iStatus))
                if not self._isCmdOk("""ERROR\t\t: Got some error in setXray."""):
                    sys.exit(1)

    def setCurrent(self, iCur=20):
        """setCurrent 0~100[10 mA]"""
        if iCur < 0 or iCur > 100:
            print """INFO\t\t: Current should within 0~100 [10 mA]"""
        else:
            self.setXray(0)
            self._send(self._interpret(3,iCur,4))
            if not self._isCmdOk("""ERROR\t\t: Got some error in setCurrent"""):
                return

    def setHV(self, iHV=350):
        """setHV 0~350[0.1 kV]"""
        if iHV < 0 or iHV > 350:
            print """INFO\t\t: High Voltage should within 0~350 [0.1 kV]"""
        else:
            self.setXray(0)
            self._send(self._interpret(3,iHV,3))
            if not self._isCmdOk("""ERROR\t\t: Got some error in setHV"""):
                sys.exit(1)

    def setGMVoltage(self, iGMV=500): #RS set to 400
        """setGMVoltage 100~600[0.1 kV] """
        if iGMV < 100 or iGMV > 600:
            print """INFO\t\t: GM Voltage should within 100~600 [1 V]"""
        else:
            self.setXray(0)
            self._send(self._interpret(3,iGMV,16))
            if not self._isCmdOk("""ERROR\t\t: You got some error in setGMVoltage"""):
                sys.exit(1)

    def setGMGate(self, iGate=10):
        """setGMGate 0~1000[0.1 second]"""
        if 1000 >= iGate >= 5:
            self._send(self._interpret(3,iGate,17))
            if not self._isCmdOk("""ERROR\t\t: You got some error in setGMGate"""):
                sys.exit(1)

    def gonioAutoCali(self):
        """Perform goniometer auto-calibartion."""
        self._send(self._interpret(13))
        time.sleep(2)
        if not self._isCmdOk("""ERROR\t\t: You got some error in gonioAutoCali"""):
            sys.exit(1)
        else:
            print """INFO\t\t: Goniometer auto-calibration done."""

    def setGonio(self, iStart=-1, iStop=-1, iStep=-1, iMode=-1, iGateTime=-1):
        """setGonio startAngle[0~3599 0.1deg] stopAngle[0~3599 0.1deg] stepSize[1~100 0.1deg] mode[1(fixCrystal)/2(fixDetector)] gateTime[0~1000 0.1sec]"""
        oBytes = self.getGonioStatus()
        if 3 > iMode > 0 and oBytes[3] != iMode:
            self._send(self._interpret(3,iMode,22))
            if not self._isCmdOk("""ERROR\t\t: You got some error in setGonio:iMode."""):
                sys.exit(1)
        if 3600 > iStart >= 0:
            self._send(self._interpret(3,iStart,11))
            if not self._isCmdOk("""ERROR\t\t: You got some error in setGonio:iStart."""):
                sys.exit(1)
        if 3600 > iStop >= 0:
            if self._hex2dec(self._getDevStatus(22)) == 1 and iStop >= 1800:
                print """INFO\t\t: Reset detector stop angle to maximum(1799)."""
                iStop = 1799
            if iStop > self._getDevStatus(13):
                iStop = self._getDevStatus(13)
                print """INFO\t\t: Stop angle beyond limit, reset to allowed maximum""", iStop
            self._send(self._interpret(3,iStop,12))
            if not self._isCmdOk("""ERROR\t\t: You got some error in setGonio:iStop."""):
                sys.exit(1)
        if 100 >= iStep > 0:
            self._send(self._interpret(3,iStep,14))
            if not self._isCmdOk("""ERROR\t\t: You got some error in setGonio:iStep."""):
                sys.exit(1)
        if 1000 >= iGateTime >= 5:
            self._send(self._interpret(3,iGateTime,17))
            if not self._isCmdOk("""ERROR\t\t: You got some error in setGonio:iGateTime"""):
                sys.exit(1)

    def getSingleValue(self):
        """getSingleValue"""
        self._send(self._interpret(modeID=0))
        return self._read()

    def setMeasurement(self, gonioMode, cryStart, detStart, iStop, iStep, iGateTime):
        """setMeasurement mode[1(fixCrystal)/2(fixDetector)] crystalStart[0~3599 0.1deg] detectorStart[0~3599 0.1deg] stopAngle[0~3599 0.1deg] stepSize[1~100 0.1deg] gateTime[0~1000 0.1s]"""
        if gonioMode == 1: #Fix crys, set det
            self.setGonio(cryStart, cryStart, 0, 2)
            self.setGonio(detStart, iStop, iStep, 1)
            self.setGMGate(iGateTime)
        elif gonioMode == 2:
            self.setGonio(detStart, detStart, 0, 1)
            self.setGonio(cryStart, iStop, iStep, 2)
            self.setGMGate(iGateTime)
        else:
            print """ERROR\t\t: You should set gonioMode to 1(fixed crystal) or 2(fixed detector)."""
            return

    def startRun(self, iStatus):
        """Start/stop measurement (stop:0, start:1)"""
        self._send(self._interpret(17-iStatus%2))
        if not self._isCmdOk("""WARNING\t\t: Something strange in startRun. Command Error."""):
            time.sleep(1)
            #sys.exit(1)
        else:
            self._measureStatus = iStatus
            if iStatus == 1:
                self._getValues()

    def _getValues(self):
        """GetValues of a continuous measurement"""
        notStop = self._read()
        output = []
        print """TimeStamp*2 Channel1*4 Channel2*4 Channel3*4""" #Output formatter
        while notStop[2] != '0x32':
            notStop = self._read()
            if notStop[2] == '0x08':
                print notStop[5:-1]
                output.append(notStop)
            elif notStop[2] == '0x31':
                print """INFO\t\t: Start getting values"""
            elif notStop[2] == '0x38':
                print """INFO\t\t: Reach limit, end of measurement."""
                self._measureStatus = 0
                return output
            else:
                print """ERROR\t\t: Unexpected readout""", notStop, """in _getValues()"""
                sys.exit(1)

    def resetAll(self):
        """resetAll"""
        self._send(self._interpret(modeID=17)) # stop measurement

    def external(self, *iCmd):
        """Call shell command by \'external your-shell-command-with-arguments\'"""
        subprocess.call(' '.join(iCmd), shell=True)

    def _interpret(self, modeID=len(_modeData)-1, data=0, devID=len(_deviceParaData)-1):
        """Intepret command to bytes"""
        "# [0x7d][Length(3+x+y bytes)][Mode(1)][[Address(2)][deviceIndex(x)]][Data(y)][0x7e]"
        command = [0x7d,0x03,0x00,0x00,0x00]
        command[2] = self._modeData[modeID][1] # mode index
        if self._modeData[modeID][2] != 0: # modeHavePara
            if self._deviceParaData[devID][0] != 'null':
                command.append(self._deviceParaData[devID][1])
        for iByte in range(1,self._modeData[modeID][3]+1):
            command.append(data%(256**(iByte))/(256**(iByte-1)))
        if modeID==3 :
            for iByte in range(1,self._deviceParaData[devID][2]+1):
                command.append(data%(256**(iByte))/(256**(iByte-1)))
        command.append(0x7e)
        command[1]=len(command)-3
        return command # list in dec

    def _send(self, command):
        """Send command via usb"""
        for i in range(len(command)):
            self._usbserial.write(struct.pack("B",command[i]))
        time.sleep(self.commtime)

    def _read(self, length=-1):
        """Read value via usb, length of output is detected automatically"""
        if length > 0:
            return [format(int(ord(i)),'#04x')for i in self._usbserial.read(length)]
        else:
            timeout = time.time()+120
            while True:
                if time.time() > timeout:
                    print "ERROR\t\t: Timeout(120s) reading value. Exit."
                    sys.exit(1)
                oBytes = [format(int(ord(i)),'#04x')for i in self._usbserial.read(2)] #Head
                if len(oBytes) > 0:
                    break
            if oBytes[0] == '0x7d' :
                oBytes += [format(int(ord(i)),'#04x')for i in self._usbserial.read(int(oBytes[1],16))] #Body
                oBytes += [format(int(ord(i)),'#04x')for i in self._usbserial.read(1)] #Tail
                return oBytes
            else:
                print """ERROR\t\t: Something wrong in reading values.""", oBytes

    def _getDevStatus(self, iDevID):
        self._send(self._interpret(modeID=4,devID=iDevID))
        iStatus = self._read()
        if iStatus[5] == format(self._deviceParaData[iDevID][1],'#04x') :
            return iStatus[6:-1][::-1] #Make slice, and reverse bytes

    def _isCmdOk(self,message):
        cmdResult = [ int(i,16) for i in self._read() ]
        if cmdResult in [self._interpret(modeID=5), self._interpret(modeID=8), self._interpret(modeID=18)]:
            return True
        else:
            print "\t\t  ", cmdResult #DEBUG
            print message
            return False

    def _hex2dec(self, iBytes):
        a=0
        for b in range(0,len(iBytes)):
            a += int(iBytes[b],16)*(256**(len(iBytes)-b-1))
        return a

### CLI
def main():
    xmethods = [f for f in dir(Xraybox) if not f.startswith('_')]
    xmethods.extend(('help','quit'))

    if len(sys.argv) == 1 :
        # Interactive mode
        print """Control X-ray box in interactive mode. Get help by 'help' method."""

        ### Initialize the box.
        iPort = raw_input('Please input the location of ttyUSB (empty=/dev/ttyUSB0): ')
        if iPort == '':
            iPort = '/dev/ttyUSB0'
        xr40 = Xraybox(iPort)

        while True:
            cmd = raw_input('Method\t\t: ').split()
            if len(cmd) == 0 or cmd[0] == 'help':
                printHelp(xmethods)
            elif cmd[0] == 'quit':
                sys.exit(0)
            elif cmd[0] in xmethods:
                if len(cmd) == 1:
                    if getattr(xr40,cmd[0]).__doc__ != None:
                        print getattr(xr40,cmd[0]).__doc__
                    if len(inspect.getargspec(getattr(xr40,cmd[0]))[0]) == 1:
                        output=getattr(xr40,cmd[0])()
                        if output != None:
                            print output
                elif cmd[0] in ['external']:
                    getattr(xr40,cmd[0])(*cmd[1:])
                else:
                    getattr(xr40,cmd[0])(*[int(x) for x in cmd[1:]])
            else:
                print """INFO\t\t: Unknown method:\"{0}\". Get help by 'help' method.""".format(' '.join(cmd))

    elif sys.argv[1] == '--help' or sys.argv[1] == '-h':
        printHelp()
    elif len(sys.argv) == 2:
        ### File input mode
        if os.path.exists(sys.argv[1]):
            f = open(sys.argv[1],"r")

            # Initialize x-ray box
            firstline = f.readline().rstrip('\n')
            if firstline.startswith('/'):
                xr40 = Xraybox(firstline)
            else:
                xr40 = Xraybox()
            f.seek(0)

            # Run line-by-line, skip blank line/unknown method/lines begin with #
            for thisline in f.readlines():
                cmd = thisline.lstrip().rstrip('\n')
                if cmd == '' or cmd.startswith('#') or cmd == firstline:
                    continue

                print """Processing\t: {0}""".format(cmd)
                cmd = cmd.split()
                if cmd[0] == 'quit':
                    break
                elif cmd[0] in xmethods:
                    if len(cmd) == 1:
                        if len(inspect.getargspec(getattr(xr40,cmd[0]))[0]) == 1:
                            output=getattr(xr40,cmd[0])()
                            if output != None:
                                print output
                    elif cmd[0] in ['external']:
                        getattr(xr40,cmd[0])(*cmd[1:])
                    else:
                        getattr(xr40,cmd[0])(*[int(x) for x in cmd[1:]])
                else:
                    print """INFO\t\t: Unknown method:\"{0}\".""".format(' '.join(cmd))

            f.close()
        else:
            print """File {0} not found!""".format(sys.argv[1])
    else:
        printHelp(xmethods)

def printHelp(methodlist):
    print """\
Help:
    \tAvailable methods can be found in the list below.
    \tAppend arguments, if needed, after the method name and separate by space.
    \tInstructions are given when running a method with no arguments.
    \t
    \tTry contact me if there's any problem. - mailto:po-hsun.chen@cern.ch
"""
    print "List of methods:\n", methodlist

if __name__ == '__main__':
    main()
