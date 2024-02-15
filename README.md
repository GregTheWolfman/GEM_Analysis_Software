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

## SRSMapping.cxx
Under the common folder, you will find a series of macros that are associated with setting the value of certain run variables to complete the analysis. This specific file is responsible for reading the mapping configuration file, so it is possible that certain aspects of the detector will need to be accounted for here. Variables that are important here could be detector shape, number of APVs connected, as well as defining detector planes. 

## SRSAPVEvent.cxx
Now, under the publisher folder, you will find the main files responsible for performing the analysis. There are a large of macros here so I will only touch on a few that are most plausible to need to be changed for detector set ups. SRSAPVEvent.cxx is used to perform the channel mapping within the APVs. This accounts for the mapping of connections between the detector, panasonic connectors, APVs, and FEC's. This code will most likely need to be altered for your experiment. Remember to check the header file too! There are a litany of functions there too

##


