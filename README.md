# SOP for X-ray tests

Editor: Yu-Wei Kao, Po-Hsun Chen
Email: ykao@cern.ch, pchen@cern.ch

# Setup the hardware

1. Put the modules on the holder, connect the cable carefully. (the cables should be below the cooling pipe)
2. Lock the window of x-ray box, turn off the light and cover the curtain.
3. Set voltage bias to be 0.15kV, leakage current should be a few uA.
4. Turn on the chiller, the temperature of modules should be around 17°C (It reads 1.066kΩ on multimeter)

# Get the CB data & Check the test parameters
Login `pixel_dev@pccmspixel186.cern.ch`
Get the latest cold box data
```sh
cd ~/pxar/data/
./getDat.sh FirstModuleName
./getDat.sh SecondModuleName
```
The script would check both retrim parameters & Dacs/Step in `tbmParameters.dat` automatically.
Create the task lists for the modules to test:
```
cd ~/XR40
./create_tasklist.sh FirstModuleName SecondModuleName
```

> Convention: FirstModule is the module close to the window, while the SecondModule is the one far away from the window.

# Timing Scan (4-8min)
The purpose of timing scan is to find proper tbm parameter set and prevent DAQ erros.
Check the tbmparameters which are found in thermal cycling tests
```sh
cd ~/pxar/data
cat MODULES/tbm*
```
In `tbmParameters_C0a.dat`, if `basea = e4` and `basee = e8`(the usual good ones), the module need not be
performed with timing scan. Otherwise, do the timing scan to see whether the default are the proper ones.
One can decide which module NOT to be tested by commenting out the command lines in script exec.sh. The
task list of `task_timing` and `task_timing_an` are for the FirstModule and SecondModule respectively.
```sh
cd ~/XR40
vi exec.sh #check which module to be tested.
./exec.sh
```
The found tbmParameters are recorded in the file `pxar_tbmParameters.log` under the directory of tested
module. If the tbmParameters are different from the default ones, a direct substitution is recommended.

# X-ray Calibration (30min)
Open the x-ray box window, setup the target and collimator.
Afterwards, lock the window and cover the x-ray box with curtain.
Then, start the calibration tests with the commands
```sh
cd ~/XR40
./xraybox.py tasks
```
During the tests, the spectrum for each target will pop out. If there is any noisy pixel found, masking them out manually is recommended.

# High Rate Efficiency Test (45min)
Open the x-ray box window, take out the target and collimator.
Afterwards, lock the window and put on the curtain.
Then, start the HR efficiency test with the commands
```sh
cd ~/XR40
./xraybox.py tasks_HR
```
A series of tests would be performed automatically: pixel alive test, re-trim test, mask dead pixels, hitmap test and efficiency tests. Monitor if there are countless errors popping out.

# [MoReWeb](https://github.com/psi46/MoReWeb) Analysis
After all the tests, the data can be analyzed with the commands
```sh
$ cd ~/DATA_HR_temp/
$ ./mkdir_xray.sh FirstModuleName
$ ./mkdir_xray.sh SecondModuleName
$ cd ~/MoReWeb_new/Analyse/
$ ./Controller.py
```

# Upload Data to Data Base
To upload data (in the format of tarball) to DB, one could use the commands
```sh
cd ~/DATA_HR_temp/
./upload.sh ModuleName_XrayHRQualification_Date_Time_TimeStamp.tar.gz
```
Example
`./upload.sh M3067_XrayHRQualification_2015-10-30_16h52m_1446220379.tar.gz`


# Termination in case of countless DAQ errors
When there are large quantity of DAQ errors popping out during the tests, stop the measurement with `Ctrl-C` and open the pXar X window(GUI) to disconnect the DTB by clicking the exit button.

> &lt;Ctrl-C&gt;
> ```sh
> cd ~/pxar
> ./bin/pXar -d data/ModuleName/ -T 35 -g
> ```
> &lt;Click exit button&gt;

Identify out the problematic module, and further investigation is required.
