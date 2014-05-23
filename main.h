/*
  	Copyright (C) 2014 Pawel Smialowski

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, see <http://www.gnu.org/licenses/>
*/
#ifndef MAIN_H
#define MAIN_H

/* D E F I N E S *************************************************************/
#define VERSION "0.1.2"
#define PRG_NAME "sff_demult"
#define FILENAME_MAX_LENGTH 1024
#define MAX_BARCODE_LENGTH 100

/* P R O T O T Y P E S *******************************************************/
void help_message(void);
void version_info(void);
void process_options(int argc, char *argv[]);
void process_sff();
void openInputFiles(char *sff_file, char *barcode_file);
void openOutputFile();
void setupBarcodeList();
void adjustReadNumber();

typedef struct listel {
	char* bc;
	int num_records;
	FILE* bcfile;
	char* bcfileName;
	struct listel *next;
} barcode_data;

#endif
