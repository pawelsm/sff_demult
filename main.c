/* L I C E N S E *************************************************************/

/*
    Copyright (C) 2014 Pawel Smialowski
    Copyright (C) 2009, 2010 Indraniel Das <indraniel@gmail.com>

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

/* I N C L U D E S ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sff.h"
#include "main.h"

/* G L O B A L S *************************************************************/
char barcode_file[FILENAME_MAX_LENGTH] = { '\0' };
char sff_file[FILENAME_MAX_LENGTH] = { '\0' };
sff_common_header h;
sff_read_header rh;
sff_read_data rd;
FILE *sff_fp, *barcode_fp;
FILE* unknownfile = NULL;
char *unknownfn = "remaining_reads.sff";
int rest;
barcode_data *curr, *head;
char barcode [MAX_BARCODE_LENGTH];
char barcodeOutName [FILENAME_MAX_LENGTH];

/* M A I N *******************************************************************/
int main(int argc, char *argv[]) {
	// Setup options
    process_options(argc, argv);
    // Initialize input files
    openInputFiles(sff_file, barcode_file);
    // Initialize output file for not matching reads
    openOutputFile();
    // Process the data
    process_sff();
    return 0;
}

/* F U N C T I O N S *********************************************************/
void
help_message() {
    fprintf(stdout, "Usage: %s %s \n", PRG_NAME, "[options]");
    fprintf(stdout, "\t%-20s%-20s\n", "-h", "This help message");
    fprintf(stdout, "\t%-20s%-20s\n", "-v", "Program and version information");
    fprintf(stdout, "\t%-20s%-20s\n", "-b", "File holding list of barcode sequences");
    fprintf(stdout, "\t%-20s%-20s\n", "-i", "SFF file holding reads");
}

void
version_info() {
    fprintf(stdout, "%s -- version: %s\n", PRG_NAME, VERSION);
}

/*
 * Takes care of options
 */
void
process_options(int argc, char *argv[]) {
    int c;
    int index;
    char *opt_b_value = NULL;
    char *opt_i_value = NULL;
    char *opt_k_value = NULL;

    while( (c = getopt(argc, argv, "hvb:i:")) != -1 ) {
        switch(c) {
            case 'h':
                help_message();
                exit(0);
                break;
            case 'v':
                version_info();
                exit(0);
                break;
            case 'b':
                opt_b_value = optarg;
                break;
            case 'i':
                opt_i_value = optarg;
                break;
            case '?':
                exit(1);
             default:
                abort();
        }
    }
    if ( opt_i_value == NULL ) {
    	fprintf(stderr, "[err] Please provide sff file using -i \n");
    	exit(1);
    }
    if ( opt_b_value == NULL ) {
    	fprintf(stderr, "[err] Please provide barcode file using -b \n");
    	exit(1);
    }
    strncpy(sff_file, opt_i_value, FILENAME_MAX_LENGTH);
    strncpy(barcode_file, opt_b_value, FILENAME_MAX_LENGTH);
}

/*
 *	Initialize input files:
 *	- sff
 *	- barcode file
 *	Barcode file should be tabulated with two columns:
 *	- first holding name of the barcode (it would be used to name files)
 *	- second with the sequence of the barcode
 *
 */
void
openInputFiles(char *sff_file, char *barcode_file) {
	if ( (sff_fp = fopen(sff_file, "r")) == NULL ) {
		fprintf(stderr, "[err] Could not open sff file '%s'.\n", sff_file);
		exit(1);
	}
	barcode_fp = fopen (barcode_file, "r");
	if (!barcode_fp) {
		fprintf (stderr, "Could not open barcode file '%s'.\n", barcode_file);
		exit(1);
	}
}

/*
 * Initialize output file for reads which are not matching any barcode
 */
void
openOutputFile() {
	unknownfile = fopen (unknownfn, "w");
	if (!unknownfile) {
		fprintf (stderr, "Could not open unknown output file '%s'.\n", unknownfn);
		exit(1);
	}
}

/*
 * Set up a linked list of the barcode data
 */
void
setupBarcodeList() {
	head = NULL;
	while (fscanf (barcode_fp, "%s%s", barcodeOutName, barcode) != EOF) {
		char *barcodeKeyTag;
		asprintf(&barcodeKeyTag,"%s%s",h.key, barcode);
		curr = (barcode_data*) malloc (sizeof (barcode_data));
		curr->bc = (char*) malloc (strlen(barcodeKeyTag) + 1);
		strcpy (curr->bc, barcodeKeyTag);
		char *barcodeOutFilename;
		asprintf(&barcodeOutFilename,"%s%s",barcodeOutName,".sff");
		curr->bcfileName = barcodeOutFilename;
		curr->bcfile = NULL;
		curr->num_records = 0;
		curr->next = head;
		head = curr;
	}
}

/*
 * Match reads to barcodes and save to according files
 * KeyTag is taken into account
 */
void
iterateReads() {
	rest = 0;
    int nbases = 0;
    char *bases;
    int numreads = (int) h.nreads;
    register int i;
    for (i = 0; i < numreads; i++) {
        read_sff_read_header(sff_fp, &rh);
        read_sff_read_data(sff_fp, &rd, h.flow_len, rh.nbases);
        nbases = (int) rh.nbases;
        /* create bases string */
        bases = get_read_bases(rd, 0, nbases);
        if (strlen(bases) > 0) {
    		curr = head;
    		while (curr) {
    			// check read against barcodes till you find match
    			if (strncmp_with_mismatch (curr->bc, bases, strlen (curr->bc), 0) == 0) {
    				break;
    			}
    			curr = curr->next;
    		}
    		/* If barcode data is found, output to demultiplexed file, else output to unknown file */
    		if (curr != NULL) {
    			if (curr->bcfile == NULL) {
    				curr->bcfile = fopen (curr->bcfileName, "w");

    				//uint32_t nreads = 0;
    				//write_sff_common_header_nreads(curr->bcfile, &h, &nreads);
    				write_sff_common_header(curr->bcfile, &h);
    			}
    			write_sff_read_header(curr->bcfile, &rh);
    			write_sff_read_data(curr->bcfile, &rd, h.flow_len, rh.nbases);
    			//fprintf (curr->bcfile, "%s\n", bases);

    			curr->num_records++;//you can use it to update number of reads
    		} else {
    			write_sff_read_header(unknownfile, &rh);
    			write_sff_read_data(unknownfile, &rd, h.flow_len, rh.nbases);
    			rest++;
    			//fprintf (unknownfile, "%s\n", bases);
    			//exit(1);
    		}
        }
        free(bases);
        free_sff_read_header(&rh);
        free_sff_read_data(&rd);
    }
}

/*
 *  Based on curr->num_records change headers of sff files to
 *  update number of reads in each file
 */
void
adjustReadNumber() {
	curr = head;
	while (curr) {
		if (curr->bcfile != NULL) {
			fprintf(stderr, "closing %s \n", curr->bcfileName);
			fclose(curr->bcfile);
			uint32_t nreads = be32toh(curr->num_records);
			fprintf(stderr, "NReads %d \n", curr->num_records);
			fprintf(stderr, "opening %s for update\n", curr->bcfileName);

			FILE *bcfile;
			bcfile=fopen (curr->bcfileName, "r+b" );
			fseek(bcfile, 20, SEEK_SET );//set indicator at nreads
			// read current value

			// write correct value
			fwrite(&nreads, sizeof(uint32_t), 1, bcfile);
			fclose(bcfile);
		}
		curr = curr->next;
	}
	fclose(unknownfile);
	uint32_t nreads = be32toh(rest);
	fprintf(stderr, "NReads %d \n", rest);
	fprintf(stderr, "opening %s for update\n", unknownfn);

	FILE *bcfile;
	unknownfile=fopen (unknownfn, "r+b" );
	fseek(unknownfile, 20, SEEK_SET );//set indicator at nreads
	// read current value

	// write correct value
	fwrite(&nreads, sizeof(uint32_t), 1, unknownfile);
	fclose(unknownfile);
}
/*
 * Main part.
 */
void
process_sff() {
	// Read and verify sff global header
    read_sff_common_header(sff_fp, &h);
    verify_sff_common_header(PRG_NAME, VERSION, &h);
    // Initialize header of output file for reads which are not matching any barcode
	write_sff_common_header(unknownfile, &h);
	// Setup list of barcodes
	setupBarcodeList();
	// Match reads to barcodes and save to according files
	iterateReads();
	// Update number of reads in each file
	adjustReadNumber();
    // Cleaning
    free_sff_common_header(&h);
    fclose(barcode_fp);
    fclose(sff_fp);
}




