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
            -b <barcode file>   File holding list of barcode sequences
            -i <fastq_file>     SFF file holding reads


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


AUTHORS
=======

Pawel Smialowski

parts of the code were originated by Indraniel Das

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
