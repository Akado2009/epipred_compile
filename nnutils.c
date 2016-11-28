/* M. Nielsen April 2002. mniel@cbs.dtu.dk */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

float ***synaps_wmtx_fp(SYNAPS * syn, FILE * fp)

{
	int     i, k, l, cnt;
	float ***m;

	m = (float ***) malloc((unsigned) (syn->nlay - 1) * sizeof(float **));

	if (!m) {
		fprintf(stderr, "Allocation failure 1 in synaps_wmtx_alloc\n");
		exit(1);
	}

	cnt = 0;

	/* NOTE. Extra element in k loop pertains to the
	   offset neuron, normally invisible to the user. */

	for (l = 0; l < syn->nlay - 1; l++) {

		printf("# Weigth matrix %d to %d, allocating %d x %d matrix\n",
			l, l + 1, syn->nnlay[l] + 1, syn->nnlay[l + 1]);

		m[l] = fmatrix(0, syn->nnlay[l + 1] - 1, 0, syn->nnlay[l]);

		if (!m[l]) {
			fprintf(stderr, "Allocation failure 2 in synaps_wmtx_alloc\n");
			exit(1);
		}

		for (k = 0; k < syn->nnlay[l + 1]; k++) 
		for (i = 0; i < syn->nnlay[l] + 1; i++) {
			fscanf(fp, "%13f", &m[l][k][i]);
			cnt++;
		}
	}

	printf("# synaps_wmtx_fp Read %d weights\n", cnt);

	return (m);
}

/* SYNAPS = Neural Network Descriptor */

SYNAPS    *synaps_alloc()

{
	SYNAPS    *p;
	if ((p = (SYNAPS *) malloc(sizeof(SYNAPS))) != NULL) {
		p->next = NULL;
		strcpy(p->nin, "nin_undefined");
		strcpy(p->nout, "nout_undefined");
		strcpy(p->wgtfile, "/dev/null");
		p->nlay = 0;
		p->nnlay = NULL;
		p->wgt = NULL;
		p->maxnper = 0;
	}
	return (p);
}

SYNAPS    *synaps_read_fp( FILE *fp )

{
	SYNAPS    *syn;
	int     np[MAXLAY];
	LINE    line;
	int     l;

	printf("# SYNAPS_Read_fp\n");

	if ( fp == NULL ) {
		printf( "SYNAPS_Read_FILE_fp. fp == NULL\n" );
		return( NULL );
	}

	/* Ignore First Line(s) */

	while ( fgets(line, sizeof line, fp) && strncmp(line+10, "LAYER:", 6 ) != 0 );

	if ((syn = synaps_alloc()) == NULL) {
		fprintf(stderr, "Cant allocate SYNAPS\n");
		exit(1);
	}

	/* Next lines determine number and length of layers */

	if ( strncmp(line+10, "LAYER:", 6 ) != 0 ) {
		return( NULL );
	} else {
		syn->nlay = 0;
#ifdef DEBUG
		printf( "# DEBUG %s", line+10 );
#endif
		sscanf(line, "%d", &np[syn->nlay]);
		syn->nlay++;

		while (fgets(line, sizeof line, fp) && 
				strncmp(line+10, "LAYER:", 6 ) ==0 ) {
#ifdef DEBUG
			printf( "# DEBUG %s", line+10 );
#endif
			sscanf(line, "%d", &np[syn->nlay]);
			syn->nlay++;
		}
	}

	syn->nnlay = ivector(0, syn->nlay - 1);

	for (l = 0; l < syn->nlay; l++)
		syn->nnlay[l] = np[l];

	syn->maxnper = 0;

	for (l = 0; l < syn->nlay; l++)
		syn->maxnper = (syn->maxnper > syn->nnlay[l] ?
			syn->maxnper : syn->nnlay[l]);

	syn->wgt = synaps_wmtx_fp(syn, fp);

	return (syn);
}

SYNAPS    *synaps_read_nfile(char *filename, int *n)

{
	FILE   *fp;
	SYNAPS  *new;
	SYNAPS	*first, *last;
	FILENAME	tmpfilename;

	printf("# SYNAPS_Read_FILE (%s)\n", filename);

	strcpy( filename, xenvfile( filename, tmpfilename) );

	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Unable to read SYN file %s\n", filename);
		exit(1);
	}

	*n=0;

	first = last = NULL;

	while ( ( new = synaps_read_fp( fp ) ) != NULL ) {

                strcpy(new->wgtfile, filename);

                if (*n == 0)
                        first = new;
                else
                        last->next = new;

                last = new;

                (*n)++;

	}

	fclose(fp);
                
        printf( "# Read %i synaps files from %s\n", *n, filename );
                
        return (first);
}            

SYNAPS    *synaps_read_file(char *filename)

{
	FILE   *fp;
	SYNAPS    *syn;
	int     np[MAXLAY];
	LINE    line;
	int     l;
	FILENAME	tmpfilename;

	printf("# SYNAPS_Read_FILE (%s)\n", filename);

	strcpy( filename, xenvfile( filename, tmpfilename) );

	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Unable to read SYN file %s\n", filename);
		exit(1);
	}

	if ((syn = synaps_alloc()) == NULL) {
		fprintf(stderr, "Cant allocate SYNAPS\n");
		exit(1);
	}

	/* Ignore First Line */

	if ( fgets(line, sizeof line, fp) == NULL ) {
		printf( "Error. Synaps file %s is empty\n", filename );
		exit( 1 );
	}

	/* Next lines determine number and length of layers */

	syn->nlay = 0;

	while (fgets(line, sizeof line, fp) && strncmp(line+10, "LAYER:", 6 ) ==0 ) {
#ifdef DEBUG
		printf( "# DEBUG %s", line+10 );
#endif
		sscanf(line, "%d", &np[syn->nlay]);
		syn->nlay++;
	}

	syn->nnlay = ivector(0, syn->nlay - 1);

	for (l = 0; l < syn->nlay; l++)
		syn->nnlay[l] = np[l];

	syn->maxnper = 0;

	for (l = 0; l < syn->nlay; l++)
		syn->maxnper = (syn->maxnper > syn->nnlay[l] ?
			syn->maxnper : syn->nnlay[l]);

	syn->wgt = synaps_wmtx_fp(syn, fp);

	fclose(fp);

	return (syn);
}

SYNAPS  *synaps_read(char *filename)

{
        FILE   *fp;
        LINE    line;
        SYNAPS    *first, *last, *new;
        int     n;
        FILENAME wgtfile;

        if ((fp = fopen(filename, "r")) == NULL) {
                fprintf(stderr, "Cannot read from %s\n", filename);
                exit(1);
        }

        first = NULL;
        n = 0;
        while (fgets(line, sizeof line, fp) != NULL) {

                if (strncmp(line, "#", 1) == 0)
                        continue;

                if (strlen(line) <= 1)
                        continue;

                if (1 != sscanf(line, "%s\n", wgtfile)) {
                        fprintf(stderr, "SYNAPS FORMAT\nWGTFILE\n%s",
                                line);
                        exit(1);
                }

                new = synaps_read_file(wgtfile);
                strcpy(new->wgtfile, wgtfile);

                if (n == 0)
                        first = new;
                else
                        last->next = new;
                last = new;

                n++;

        }

        fclose(fp);

        printf( "# Read %i synaps files from %s\n", n, filename );

        return (first);
}

SYNAPS  *synaps_read_pfix(char *filename, char *prefix)

{
        FILE   *fp;
        LINE    line;
        SYNAPS    *first, *last, *new;
        int     n;
        FILENAME wgtfile, tmpfile;

        if ((fp = fopen(filename, "r")) == NULL) {
                fprintf(stderr, "Cannot read from %s\n", filename);
                exit(1);
        }

        first = NULL;
        n = 0;

        while (fgets(line, sizeof line, fp) != NULL) {

                if (strncmp(line, "#", 1) == 0)
                        continue;

                if (strlen(line) <= 1)
                        continue;

                if (1 != sscanf(line, "%s\n", wgtfile)) {
                        fprintf(stderr, "SYNAPS FORMAT\nWGTFILE\n%s",
                                line);
                        exit(1);
                }

		if ( strlen( prefix ) > 0 ) {

			sprintf( tmpfile, "%s/%s", prefix, wgtfile );
			strcpy( wgtfile, tmpfile );
		}

                new = synaps_read_file(wgtfile);
                strcpy(new->wgtfile, wgtfile);

                if (n == 0)
                        first = new;
                else
                        last->next = new;
                last = new;

                n++;

        }

        fclose(fp);

        printf( "# Read %i synaps files from %s\n", n, filename );

        return (first);
}

/* SYNAPS_HOW = HOW Neural Network Descriptor */

float ***synaps_how_wmtx_fp(SYNAPS_HOW * syn, FILE * fp)

{
	int     i, k, l, cnt;
	float ***m;

	m = (float ***) malloc((unsigned) (syn->nlay - 1) * sizeof(float **));

	if (!m) {
		fprintf(stderr, "Allocation failure 1 in synaps_how_wmtx_alloc\n");
		exit(1);
	}

	cnt = 0;

	/* NOTE. Extra element in k loop pertains to the
	   offset neuron, normally invisible to the user. */

	for (l = 0; l < syn->nlay - 1; l++) {

		printf("# Weigth matrix %d to %d, allocating %d x %d matrix\n",
			l, l + 1, syn->nnlay[l] + 1, syn->nnlay[l + 1]);

		m[l] = fmatrix(0, syn->nnlay[l + 1] - 1, 0, syn->nnlay[l]);

		if (!m[l]) {
			fprintf(stderr, "Allocation failure 2 in synaps_how_wmtx_alloc\n");
			exit(1);
		}

		for (k = 0; k < syn->nnlay[l + 1]; k++) 
		for (i = 0; i < syn->nnlay[l] + 1; i++) {
			fscanf(fp, "%13f", &m[l][k][i]);
			cnt++;
		}
	}

	printf("# synaps_how_wmtx_fp Read %d weights\n", cnt);

	return (m);
}

float ***synaps_how_wmtx_linelist(SYNAPS_HOW * syn, LONGLINELIST *list )

{
	int     i, k, l, cnt, nc, j;
	float ***m, *vec;
	LONGLINELIST	*ln;

	m = (float ***) malloc((unsigned) (syn->nlay - 1) * sizeof(float **));

	if (!m) {
		fprintf(stderr, "Allocation failure 1 in synaps_how_wmtx_alloc\n");
		exit(1);
	}

	cnt = 0;

	/* NOTE. Extra element in k loop pertains to the
	   offset neuron, normally invisible to the user. */

	ln = list;

	j = 0;
	vec = fvector_split( ln->line, &nc );

	for (l = 0; l < syn->nlay - 1; l++) {

		printf("# Weigth matrix %d to %d, allocating %d x %d matrix\n",
			l, l + 1, syn->nnlay[l] + 1, syn->nnlay[l + 1]);

		m[l] = fmatrix(0, syn->nnlay[l + 1] - 1, 0, syn->nnlay[l]);

		if (!m[l]) {
			fprintf(stderr, "Allocation failure 2 in synaps_how_wmtx_alloc\n");
			exit(1);
		}

		for (k = 0; k < syn->nnlay[l + 1]; k++) 
		for (i = 0; i < syn->nnlay[l] + 1; i++) {

			if ( j < nc )
				m[l][k][i] = vec[j++];
			else {
				fvector_free( vec, 0, nc-1 );

				ln = ln->next;
				j = 0;
				if ( ln )
					vec = fvector_split( ln->line, &nc );
				else {
					printf( "Error. Wrong format of synaps file %i %i\n", cnt,nc );
					exit( 1 );
				}
				m[l][k][i] = vec[j++];
			}
			cnt++;
		}
	}

	printf("# synaps_how_wmtx_linelist Read %d weights\n", cnt);

	return (m);
}

SYNAPS_HOW    *synaps_how_alloc()

{
	SYNAPS_HOW    *p;
	if ((p = (SYNAPS_HOW *) malloc(sizeof(SYNAPS_HOW))) != NULL) {
		p->next = NULL;
		strcpy(p->nin, "nin_undefined");
		strcpy(p->nout, "nout_undefined");
		strcpy(p->wgtfile, "/dev/null");
		p->nlay = 0;
		p->nnlay = NULL;
		p->wgt = NULL;
		p->maxnper = 0;
		strcpy(p->alphain, "alphain_undefined" );
		strcpy(p->alphaout, "alphaout_undefined" );
		p->wsize = -99;
		p->lenpos = -99;
		p->lsize = -99;
		p->lcomp = -99;
		p->pfac = 1.0;
		p->iact = -1;
		p->iprof = -1;
		p->weight = 1.0;
		p->nialph = -99;
		p->input = -99;
	}
	return (p);
}

SYNAPS_HOW    *synaps_how_read_file(char *filename)

{
	FILE   *fp;
	SYNAPS_HOW    *syn;
	int     np[MAXLAY];
	LINE    line;
	int     l, i;
        FILENAME tmpfilename;

        strcpy( filename, xenvfile( filename, tmpfilename) );


	printf("# SYNAPS_HOW_Read_FILE (%s)\n", filename);

	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Unable to read SYN file %s\n", filename);
		exit(1);
	}

	if ((syn = synaps_how_alloc()) == NULL) {
		fprintf(stderr, "Cant allocate SYNAPS_HOW\n");
		exit(1);
	}

	/* First Line contains alphain and alphaout */

	if ( fgets(line, sizeof line, fp) == NULL ) {
		printf( "Error reading SYNAPS_HOW file %s Line %s\n", filename ,line );
		exit( 1 );
	}
	else {
		i=0;
		while ( i< strlen( line ) && line[i] != ':' ) i++;

		if ( line[i] != ':' ) {
			printf( "Error II reading SYNAPS_HOW file %s Line %s\n", filename ,line );
			exit( 1 );
		}
		i++;
		if ( sscanf(line+i, "%s OUT: %s", syn->alphain, syn->alphaout ) != 2 ) {
			printf( "Error reading SYNAPS_HOW file %s Line %s\n", filename ,line+i );
			exit( 1 );
		}
	}

	/* Next lines determine number and length of layers */

	syn->nlay = 0;

	while (fgets(line, sizeof line, fp) && strncmp(line+10, "LAYER:", 6 ) ==0 ) {
		sscanf(line, "%d", &np[syn->nlay]);
		syn->nlay++;
	}

	if ( strstr( line, "NWSIZE" ) != NULL ) {
		if ( sscanf( line, "%*i %*s %i", &(syn->wsize) )  != 1 ) {
			printf( "Error. Cannot read NWSIZE from synaps file\n" );
			exit( 1 );
		}
	}

	syn->nnlay = ivector(0, syn->nlay - 1);

	for (l = 0; l < syn->nlay; l++)
		syn->nnlay[l] = np[l];

	syn->maxnper = 0;

	for (l = 0; l < syn->nlay; l++)
		syn->maxnper = (syn->maxnper > syn->nnlay[l] ?
			syn->maxnper : syn->nnlay[l]);

	syn->wgt = synaps_how_wmtx_fp(syn, fp);

	fclose(fp);

	return (syn);
}


SYNAPS_HOW      *synaps_how_read(char *filename)

{
        FILE   *fp;
        LINE    line;
        SYNAPS_HOW    *first, *last, *new;
        int     n;
        FILENAME wgtfile;

        if ((fp = fopen(filename, "r")) == NULL) {
                fprintf(stderr, "Cannot read from %s\n", filename);
                exit(1);
        }

        first = NULL;
        n = 0;
        while (fgets(line, sizeof line, fp) != NULL) {

                if (strncmp(line, "#", 1) == 0)
                        continue;

                if (strlen(line) <= 1)
                        continue;

                if (1 != sscanf(line, "%s\n", wgtfile)) {
                        fprintf(stderr, "SYNAPS_HOW FORMAT\nWGTFILE\n%s",
                                line);
                        exit(1);
                }

                new = synaps_how_read_file(wgtfile);
                strcpy(new->wgtfile, wgtfile);

                if (n == 0)
                        first = new;
                else
                        last->next = new;
                last = new;

                n++;

        }

        fclose(fp);

        printf( "# Read %i How synaps files from %s\n", n, filename );

        return (first);
}

SYNAPS_HOW    *synaps_how_read_file_new(char *filename)

{
	FILE   *fp;
	SYNAPS_HOW    *syn;
	int     np[MAXLAY];
	LINE    line;
	int     l, i;
	FILENAME tmpfilename;

	strcpy( filename, xenvfile( filename, tmpfilename) );

	printf("# SYNAPS_HOW_Read_FILE (%s)\n", filename);

	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Unable to read SYN file %s\n", filename);
		exit(1);
	}

	if ((syn = synaps_how_alloc()) == NULL) {
		fprintf(stderr, "Cant allocate SYNAPS_HOW\n");
		exit(1);
	}

	/* First Line contains alphain and alphaout */

	if ( fgets(line, sizeof line, fp) == NULL ) {
		printf( "Error reading SYNAPS_HOW file %s Line %s\n", filename ,line );
		exit( 1 );
	}
	else {
		i=0;
		while ( i< strlen( line ) && line[i] != ':' ) i++;

		if ( line[i] != ':' ) {
			printf( "Error II reading SYNAPS_HOW file %s Line %s\n", filename ,line );
			exit( 1 );
		}
		i++;
		if ( sscanf(line+i, "%s OUT: %s", syn->alphain, syn->alphaout ) != 2 ) {
			printf( "Error reading SYNAPS_HOW file %s Line %s\n", filename ,line+i );
			exit( 1 );
		}
	}

	/* Next lines determine number and length of layers */

	syn->nlay = 0;

	while (fgets(line, sizeof line, fp) && strncmp(line+10, "LAYER:", 6 ) ==0 ) {
		sscanf(line, "%d", &np[syn->nlay]);
		syn->nlay++;
	}

	/* Next line contains the window size */

	if ( strstr( line, "NWSIZE" ) != NULL ) {
		if ( sscanf( line, "%*i %*s %i", &(syn->wsize) )  != 1 ) {
			printf( "Error. Cannot read NWSIZE from synaps file\n" );
			exit( 1 );
		}
	}

	/* Next line contains the lenpos, lsize, and lcomp */

	if ( fgets(line, sizeof line, fp) == NULL || strstr( line, "LENPOS" ) == NULL ) {
		printf( "Error. Cannot read LENPOS from synaps file\n" );
		exit( 1 );
	}

	if ( sscanf( line, "%i :LENPOS %i :LSIZE %i :LCOMPO", 
		&(syn->lenpos), &(syn->lsize), &(syn->lcomp ) ) != 3 ) {
		printf( "Error. Wrong format of LENPOS line %s\n", line );
		exit( 1 );
	}

	/* Next line contains NIALPHA and input */ 
	/* These value arec not used */

        if ( fgets(line, sizeof line, fp) == NULL ) {
		printf( "Error. Cannot read NIALPH from synaps file\n" );
		exit( 1 );
	}

	/* Next line contains pfac, iact and iprof */

	if ( fgets(line, sizeof line, fp) == NULL || strstr( line, "PFAC" ) == NULL ) {
                printf( "Error. Cannot read PFAC from synaps file\n" );
                exit( 1 );
        }

        if ( sscanf( line, "%f :PFAC %i :IACT %i :IPROF", 
                &(syn->pfac), &(syn->iact), &(syn->iprof ) ) != 3 ) {
                printf( "Error. Wrong format of PFAC line %s\n", line );
                exit( 1 );                                                                          
        }                                                                          

	syn->nnlay = ivector(0, syn->nlay - 1);

	for (l = 0; l < syn->nlay; l++)
		syn->nnlay[l] = np[l];

	syn->maxnper = 0;

	for (l = 0; l < syn->nlay; l++)
		syn->maxnper = (syn->maxnper > syn->nnlay[l] ?
			syn->maxnper : syn->nnlay[l]);

	syn->wgt = synaps_how_wmtx_fp(syn, fp);

	fclose(fp);

	return (syn);
}

SYNAPS_HOW    *synaps_how_read_file_new2(char *filename)

{
	SYNAPS_HOW    *syn;
	int     np[MAXLAY];
	LINE    line;
	int     l, i;
	LONGLINELIST *linelist, *ln;
	FILENAME tmpfilename;

        strcpy( filename, xenvfile( filename, tmpfilename) );

	printf("# SYNAPS_HOW_Read_FILE (%s)\n", filename);

	linelist = longlinelist_read( filename );

	if ( linelist == NULL ) {
		printf( "Error. cannot read synaps file %s\n", filename );
		exit( 1 );
	}

	if ((syn = synaps_how_alloc()) == NULL) {
		fprintf(stderr, "Cant allocate SYNAPS_HOW\n");
		exit(1);
	}

	ln = linelist;

	/* First Line contains alphain and alphaout */

	if ( ln == NULL ) {
		printf( "Error reading SYNAPS_HOW file Alphain and Alphaout %s Line %s\n", filename ,line );
		exit( 1 );
	}
	else {
		i=0;
		while ( i< strlen( ln->line ) && ln->line[i] != ':' ) i++;

		if ( ln->line[i] != ':' ) {
			printf( "Error II reading SYNAPS_HOW file %s Line %s\n", filename ,ln->line );
			exit( 1 );
		}
		i++;
		if ( sscanf(ln->line+i, "%s OUT: %s", syn->alphain, syn->alphaout ) != 2 ) {
			printf( "Error reading SYNAPS_HOW file %s Line %s\n", filename ,ln->line+i );
			exit( 1 );
		}
	}

	/* Next lines determine number and length of layers */

	syn->nlay = 0;

	ln = ln->next;

	while (ln && strncmp(ln->line+10, "LAYER:", 6 ) ==0 ) {
		sscanf(ln->line, "%d", &np[syn->nlay]);
		syn->nlay++;
		ln = ln->next;
	}

	/* Next line contains the window size */

	if ( ln && strstr( ln->line, "NWSIZE" ) != NULL ) {
		if ( sscanf( ln->line, "%*i %*s %i", &(syn->wsize) )  != 1 ) {
			printf( "Error. Cannot read NWSIZE from synaps file\n" );
			exit( 1 );
		}
		ln = ln->next;
	}


	/* Next line contains the lenpos, lsize, and lcomp */

	if ( ln && strstr( ln->line, "LENPOS" ) != NULL ) {

		if ( sscanf( ln->line, "%i :LENPOS %i :LSIZE %i :LCOMPO", 
			&(syn->lenpos), &(syn->lsize), &(syn->lcomp ) ) != 3 ) {
			printf( "Error. Wrong format of LENPOS line %s\n", ln->line );
			exit( 1 );
		}

		ln = ln->next;
	}
	else {
		syn->lenpos = 0;
		syn->lsize = 0;
		syn->lcomp = 0;
	}

	/* Next line contains NIALPHA and input */ 
	/* These value arec not used */

        if ( ln && strstr( ln->line, "NIALPH" ) != NULL ) {

		if ( sscanf( ln->line, "%i :NIALPH %i :INPUT",
                        &(syn->nialph), &(syn->input) ) != 2 ) {
                        printf( "Error. Wrong format of NIALPH line %s\n", line );
                        exit( 1 );
                }
                 
		ln = ln->next;
	}
	else {
		syn->nialph = strlen( syn->alphain );
	}

	/* Next line contains pfac, iact and iprof */

	if ( ln && strstr( ln->line, "PFAC" ) != NULL ) {
        	if ( sscanf( ln->line, "%f :PFAC %i :IACT %i :IPROF", 
                	&(syn->pfac), &(syn->iact), &(syn->iprof ) ) != 3 ) {
                	printf( "Error. Wrong format of PFAC line %s\n", line );
                	exit( 1 );                                                                          
        	}              	                                                            

		ln = ln->next;
	}
	else {
		syn->pfac = 1;
		syn->iact = 1;
		syn->iprof = 1;
	}

	syn->nnlay = ivector(0, syn->nlay - 1);

	for (l = 0; l < syn->nlay; l++)
		syn->nnlay[l] = np[l];

	syn->maxnper = 0;

	for (l = 0; l < syn->nlay; l++)
		syn->maxnper = (syn->maxnper > syn->nnlay[l] ?
			syn->maxnper : syn->nnlay[l]);

	syn->wgt = synaps_how_wmtx_linelist(syn, ln);

	longlinelist_free( linelist );

	return (syn);

}

SYNAPS_HOW      *synaps_how_read_new(char *filename)

{
        FILE   *fp;
        LINE    line;
        SYNAPS_HOW    *first, *last, *new;
        int     n;
        FILENAME wgtfile;
	float	weight;
	int	nc;
        FILENAME tmpfilename;

        strcpy( filename, xenvfile( filename, tmpfilename) );

        if ((fp = fopen(filename, "r")) == NULL) {
                fprintf(stderr, "Cannot read from %s\n", filename);
                exit(1);
        }

        first = NULL;
        n = 0;
        while (fgets(line, sizeof line, fp) != NULL) {

                if (strncmp(line, "#", 1) == 0)
                        continue;

                if (strlen(line) <= 1)
                        continue;

                if ( ( nc = sscanf(line, "%s %f\n", wgtfile, &weight) ) < 1 ) {
                        fprintf(stderr, "SYNAPS_HOW FORMAT\nWGTFILE\n%s",
                                line);
                        exit(1);
                }

		if ( nc == 1 )
			weight = 1.0;

#ifdef OLD
                new = synaps_how_read_file_new(wgtfile);
#else
		new = synaps_how_read_file_new2(wgtfile);
#endif
                strcpy(new->wgtfile, wgtfile);

		new->weight = weight;

                if (n == 0)
                        first = new;
                else
                        last->next = new;
                last = new;

                n++;

        }

        fclose(fp);

        printf( "# Read %i How synaps files from %s\n", n, filename );

        return (first);
}
