# Introduction
This software (Adonis) was developed to analyze and characterize GEM detectors at Florida Tech. Using root data files that are generated using AmoreSRS to interface with the GEM detectors, Adonis can find find basic hit characteristics (strip multiplicity, gain, cluster distributions and size) as well as track reconstruction characteristics like spatial resolution

Here is a list of steps to help run and undertsand this software

First, I will show how to use AmoreSRS to load and read data. This step assumes youve already measured the data and they are stored in a .raw file

# Using AmoreSRS
There are many analysis actions that can be accomplished with amoreSRS, in this tutorial, I will only discuss how to use AmoreSRS to convert .raw files into structured .root files. To start, I will give a small overview on using AmoreSRS. There are multiple macros within AmoreSRS that need to be altered if a user were to add a new detector type. AmoreSRS is made of multiple smaller codes that control different aspects of the anaylsis.

##Amore Configuration File (.cfg)
This is the main file for configuring your usage of Amore. Here you control how the data is being analyzed (which runtype: pedestal, physics, rootfileoutput?), what other files need to be loaded for this detector (mapping files, pedestal files, results output directory...), as well as setting analysis specific variables (crosstalk flag, clustering parameters, zero supression level..). 

##Mapping File (.cfg)
This file is for storing the information attributed to the detector setup. Here you will define what detectors are being used, what APV connections are made, as well as FEC indicies. You will use keywords to describe the readout pattern and the detector planes that will be used to determine which mappings are applied later in other macros. 


