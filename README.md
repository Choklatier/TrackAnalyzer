# Track Analyzer

The code is written to be used in [an old VMs to access CMS Open data from Run1](https://opendata.cern.ch/record/250). Datasets like the [2010B MinimumBias](https://opendata.cern.ch/record/42) can be ran on using this setup.

[This tutorial](https://opendata.cern.ch/docs/cms-virtual-machine-2010) indicates how to setup the CMS software.\ 
As a short summary, once the VM is acquired:
```
cmsrel CMSSW_4_2_8 
cd CMSSW_4_2_8/src/
cmsenv
```
Then copy the content of this repository into this path. Compiling and running can be done by executing the following:
```
scram b
cmsRun python/trackanalyzer_cfi.py
```

## Code Editing tips
In order to have a better experience editing the code, the repository may be cloned outside the VM and the files transfered to the VM. An example of how to do so is as follows:\
In the VM, attribute a local ip address to the VM:
```
sudo ifconfig eth1 192.168.56.10 netmask 255.255.255.0 up
```
Then use scp to transfer files to the VM. Here are a few examples:
```
scp src/TrackAnalyzer.cc cms-opendata@192.168.56.10:~/CMSSW_4_2_8/src/LLP/TrackAnalyzer/src/
scp python/trackanalyzer_cfi.py cms-opendata@192.168.56.10:~/CMSSW_4_2_8/src/LLP/TrackAnalyzer/python/
scp -r ./datasets/ cms-opendata@192.168.56.10:~/CMSSW_4_2_8/src/LLP/TrackAnalyzer/
```