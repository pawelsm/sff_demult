SYNOPSIS
========

The program `sff_demult` splits provided sff file based on list of the barcodes as specified in tabulated file.

USAGE
=====

Given an SFF file, `file.sff` and barcode `barcodes.txt` file you can simply run:

    sff_demult -i file.sff -b barcodes.txt

The barcode file should be tabulated as shown below:

IonXpress_001	CTAAGGTAAC


Options
-------

Below is the help message (via `sff_demult -h`) describing its usage & options:

Usage: sff_demult [options] 
	-h                  This help message
	-v                  Program and version information
	-b                  File holding list of barcode sequences
	-i                  SFF file holding reads


INSTALLATION
============

The installation process currently consists of a very simple Makefile.

Just do the following:

    git clone git://github.com/pawelsm/sff_demult.git
    cd sff_demult
    make 

or
    download
    wget https://github.com/pawelsm/sff_demult/archive/master.zip
    
    unzip
    unzip master.zip

    cd sff_demult
    make

The `sff_demult` executable should be in the working directory.
Afterwards, you can move the executable to wherever you wish.

NOTES
=====

Without any given options the default approach is to output trimmed
sequence and quality values.  This is similar in nature to the sff tools
produced by 454 Life Sciences/Roche.

AUTHORS
=======

Pawel Smialowski
Some parts of the code were originated by
Indraniel Das (indraniel@gmail.com)

Contributors
------------

* [Pawel Smialowski](https://github.com/pawelsm)

ACKNOWLEDGEMENTS
================

This software was developed at The LMU
Ludwig Maximilian University of Munich

DISCLAIMER
==========

This software is provided "as is" without warranty of any kind.

May 23, 2014
