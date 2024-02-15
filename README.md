# Introduction
This software (Adonis) was developed to analyze and characterize GEM detectors at Florida Tech. Using root data files that are generated using AmoreSRS to interface with the GEM detectors, Adonis can find find basic hit characteristics (strip multiplicity, gain, cluster distributions and size) as well as track reconstruction characteristics like spatial resolution

Here is a list of steps to help run and undertsand this software

First, I will show how to use AmoreSRS to load and read data. This step assumes youve already measured the data and they are stored in a .raw file

# Using AmoreSRS
There are many analysis actions that can be accomplished with amoreSRS, in this tutorial, I will only discuss how to use AmoreSRS to convert .raw files into structured .root files. To start, I will give a small overview on using AmoreSRS. There are multiple macros within AmoreSRS that need to be altered if a user were to add a new detector type. AmoreSRS is made of multiple smaller codes that control different aspects of the anaylsis. First, I will discuss the use of configuration files to set up the analysis you plan to perform. These files can be found in the configFileDir folder in AmoreSRS. 

## Amore Configuration File (.cfg)
This is the main file for configuring your usage of Amore. Here you control how the data is being analyzed (which runtype: pedestal, physics, rootfileoutput?), what other files need to be loaded for this detector (mapping files, pedestal files, results output directory...), as well as setting analysis specific variables (crosstalk flag, clustering parameters, zero supression level..). 

## Mapping File (.cfg)
This file is for storing the information attributed to the detector setup. Here you will define what detectors are being used, what APV connections are made, as well as FEC indicies. You will use keywords to describe the readout pattern and the detector planes that will be used to determine which mappings are applied later in other macros. 

There are two other configration files that are used by AmoreSRS, histconfig and dispconfig. But these two configuration files are associated with setting up the AmoreGUI for visualizing the data. Since we only use AmoreSRS to convert .raw files into .root files, I wont go into much detail about them. Now I will discuss the more inportant macros within AmoreSRS that will need to be altered to add a new Detector to it. These files can be found under the src folder in AmoreSRS. Note that these c++ files also come with .h header files that include some functions as well.

## SRSMapping
Under the common folder, you will find a series of macros that are associated with setting the value of certain run variables to complete the analysis. This specific file is responsible for reading the mapping configuration file, so it is possible that certain aspects of the detector will need to be accounted for here. Variables that are important here could be detector shape, number of APVs connected, as well as defining detector planes. 

## SRSAPVEvent
Now, under the publisher folder, you will find the main files responsible for performing the analysis. There are a large of macros here so I will only touch on a few that are most plausible to need to be changed for detector set ups. SRSAPVEvent.cxx is used to perform the channel mapping within the APVs. This accounts for the mapping of connections between the detector, panasonic connectors, APVs, and FEC's. This code will most likely need to be altered for your experiment. Remember to check the header file too! There are a litany of functions there too

## SRSHit
This file is responsible for calculating hit information. Here there are a series of functions for hit objects such as APV id per event or hit timebin. One important functino here is SetStripNo, which is used to convert chNo to the strip number in the Detctor. This is an important mapping step to ensure the hit is reconstructed in the correct geometric location. 

## SRSOutputROOT
SRSOutputROOT is another important macro especially for this analysis since we are using Amore primarily as a means of converting the data to a .root file. In this macro, you can add or remove information to the root file. This can be important for a software designed specifically for a detector type because you can chose which data are important for the analysis.

These are the main internal files that will need to be checked into when adding a new detector configuration. With these in order, you can finally save the data in a more usable way for other software. I will now move onto using Amore

# Generating Data Files
Using the terminal, we can now use AmoreSRS to fill usable data files. This tutorial will assume amore is already installed and running on the computer. I will also go over some generally useful commands in Amore to aid in using this software. To use amore there are two main terminal commands; the first command is `make install`.
```
cd ~/Your_amoreSRS_Directory
make install
```
This command is used to build amoreSRS. This is necessary to do whenever changes are made to the internal macros (SRS-Anything.cxx). The only files that are reloaded everytime amore is used are the configuration files (Mapping and Amore). With your amoreSRS version ready to use on data, the first thing to do is ensure the configuration files are corrcet for whatever analysis youre doing with this data. Using the Amore configuration file, uncomment the run parameters necessary for the detector. For example, I wll show the process for aquiring the root data file. The first step for most analyses is to generate a pedestal file. This is done by first uncommenting the RawPedestal runtype in Amore config and running the second necessary command to start amoreSRS.
```
cd ~/Your_amoreSRS_Directory
amoreAgent -a SRS01 -s /path/to/Pedestal_data.raw -e 1000 -c 5
```
The first few parts of this command `amoreAgent -a SRS01 -s` are for starting the correct amoreSRS agent and are going to stay constant anytime this command is used. `/path/to/Pedestal_data.raw` is of course the path from the current directory to the data file. Finally, the last part `-e 1000 -c 5` stands for 1000 events over 5 cycles. Thses numbers can change depending on the number of events stored in your data files. The number of cycles essentially determines how often the histograms are filled in the GUI.

With the RawPedestal run now over, repeat the same command with the runtype set to Pedestal now instead of RawPedestal. The first RawPedestal run is to load the histograms with pedstal data for use in subtracting off real data and determining hit locations. The second Pedestal run is to generate a pedestal file and correct a commonmode for each channel. This means the noise fluctuations are removed so the average noise level is zero. Next, we can finally analyze a data file to determine hit locations and save the information to a root file. From here, there are two main run types that can be used, Phyiscs and Rootfile. The physics runtype will locate hits and generate a root file that is filled with hit characteristics such as cluster size distributions, Hit maps, or charge ratio plots. However, this runtype does require the zerosup flag to be set to a non-zero, positive value. We generally set this value to 5 as this parameter sets the sigma value for hit tolerances (hit charge > pedstal avg noise + (zerosup * pedestal std dev)). The second runtype, Rootfile, is used to take the detecor hit data and store it in a rootfile. To get a better idea of the organization within the rootfile, check out the different fill_tree functions in SRSOutputROOT. Once familiar, set the runtype to RootFile and run the command once more, but with a Data file this time.
```
amoreAgent -a SRS01 -s /path/to/RealPhysics_data.raw -e 1000 -c 5
```
In the amore config file, there is a output rootfile directory and name option. Use this section to find your data file. One last step is necessary to use Adonis next to find Detector characteristics. Adonis uses its own pedestal file to determine hit locations, so we need to run the command once more with a pedestal file, but still using the RootFile runtype. Remember to rename the data file as to not overwrite your data!
```
amoreAgent -a SRS01 -s /path/to/Pedestal_data.raw -e 1000 -c 5
```
With a Pedestal and Data file in hand, you can now use the adonis code with is stored in APVEvent.C above.





