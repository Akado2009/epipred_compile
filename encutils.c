/* M. Nielsen November 2002. mniel@cbs.dtu.dk */

/*      ENC Utilities
        Various Routines to do sequence encoding for NN
        training
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "utils.h"

float	**read_pscplmat( FILENAME filename, char **alphabet )

{
	float	**m;
	int	i,nn;
	FILE	*fp;
	int	fc, ff;
	LINE	line;
	WORD	let;

	if ( ( fp = stream_input( filename, &fc, &ff ) ) == NULL ) {
		printf( "Error. Cannot open file %s\n",  filename );
		exit( 1 );
	}

	i=0;
	m = fmatrix( 0, 8, 0, 19 );
	(*alphabet) = cvector( 0, 20 );

	while ( fgets( line, sizeof line, fp ) != NULL ) {
		
		if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "#", 1 ) == 0 ) continue;

		if ( i>19 ) continue;

		if ( ( nn = sscanf( line, "%s %f %f %f %f %f %f %f %f %f", let, 
			&m[0][i], &m[1][i], &m[2][i],
			&m[3][i], &m[4][i], &m[5][i],
			&m[6][i], &m[7][i], &m[8][i] ) ) != 10 ) {
			printf( "Error reading PSCPL matrix. line %s %i %i. Exit\n", line, i, nn );
			exit( 1 );
		}

		(*alphabet)[i] = let[0];

		i++;
	}

	(*alphabet)[20] = 0;

	printf( "# PSCPL matrix read. Alphabet %s\n", *alphabet );

	stream_close( fp, fc, filename );

	return( m );

}

float	**hmm_matrix_read( FILENAME filename, char **alphabet )

{
	FILE            *fp;
        int             fc, ff;
        LINE            line;
	WORD		*wvec;
	float		**m;
	int		i;
	int		alphabet_read;

	if ( ( fp = stream_input( filename, &fc, &ff ) ) == NULL ) {
		printf( "Error. Cannot open file %s\n", filename );
		exit( 1 );
	}

	(*alphabet) = cvector( 0, 20 );
	wvec = wvector( 0, 19 );
	m = fmatrix( 0, 8, 0, 19 );

	alphabet_read = 0;

	while ( fgets( line, sizeof line, fp ) != NULL ) {
		
		if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "#", 1 ) == 0 ) continue;

		if ( strncmp( line, "Last", 4 ) == 0 ) continue;

#define SEQFORMAT "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"
		if ( ! alphabet_read && sscanf( line, SEQFORMAT, 
			wvec[0], wvec[1], wvec[2], wvec[3], wvec[4],
			wvec[5], wvec[6], wvec[7], wvec[8], wvec[9],
			wvec[10], wvec[11], wvec[12], wvec[13], wvec[14],
			wvec[15], wvec[16], wvec[17], wvec[18], wvec[19] ) == 20 ) {

			for ( i=0;i<20;i++ ) 
				(*alphabet)[i] = wvec[i][0];
		
			i=0;

			alphabet_read = 1;

			continue;
		}

#define MFORMAT "%*i %*s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
		if ( sscanf( line, MFORMAT,
			&m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4],
			&m[i][5], &m[i][6], &m[i][7], &m[i][8], &m[i][9],
			&m[i][10], &m[i][11], &m[i][12], &m[i][13], &m[i][14],
			&m[i][15], &m[i][16], &m[i][17], &m[i][18], &m[i][19] ) == 20 ) {

			i++;

			continue;
		}

	}

	(*alphabet)[20] = 0;

	printf( "# Hmm matrix file %s read. Alphabet %s\n", filename, (*alphabet) );
	
	stream_close( fp, fc, filename );

	return( m );
}

float   **hmm_matrix_read_fp( FILE *fp, char **alphabet )

{
        LINE            line;
	WORD		*wvec;
	float		**m;
	int		i;
	int		alphabet_read;

	(*alphabet) = cvector( 0, 20 );
	wvec = wvector( 0, 19 );
	m = fmatrix( 0, 8, 0, 19 );

	alphabet_read = 0;

	while ( fgets( line, sizeof line, fp ) != NULL ) {
		
		if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "#", 1 ) == 0 ) continue;

		if ( strncmp( line, "Last", 4 ) == 0 ) continue;

#define SEQFORMAT "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"
		if ( ! alphabet_read && sscanf( line, SEQFORMAT, 
			wvec[0], wvec[1], wvec[2], wvec[3], wvec[4],
			wvec[5], wvec[6], wvec[7], wvec[8], wvec[9],
			wvec[10], wvec[11], wvec[12], wvec[13], wvec[14],
			wvec[15], wvec[16], wvec[17], wvec[18], wvec[19] ) == 20 ) {

			for ( i=0;i<20;i++ ) 
				(*alphabet)[i] = wvec[i][0];
		
			i=0;

			alphabet_read = 1;

			continue;
		}

#define MFORMAT "%*i %*s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
		if ( sscanf( line, MFORMAT,
			&m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4],
			&m[i][5], &m[i][6], &m[i][7], &m[i][8], &m[i][9],
			&m[i][10], &m[i][11], &m[i][12], &m[i][13], &m[i][14],
			&m[i][15], &m[i][16], &m[i][17], &m[i][18], &m[i][19] ) == 20 ) {

			i++;

			continue;
		}

	}

	(*alphabet)[20] = 0;

	printf( "# Hmm matrix read from FP. Alphabet %s\n", (*alphabet) );
	
	return( m );
}

float	**hmm_matrix_read_l( FILENAME filename, char **alphabet, int *l )

{
	FILE            *fp;
        int             fc, ff;
        LINE            line;
	WORD		*wvec;
	float		**m, a;
	int		i;
	int		alphabet_read;
	WORD		aa;

	if ( ( fp = stream_input( filename, &fc, &ff ) ) == NULL ) {
		printf( "Error. Cannot open file %s\n", filename );
		exit( 1 );
	}

	(*alphabet) = cvector( 0, 20 );
	wvec = wvector( 0, 19 );

	alphabet_read = 0;
	(*l) = 0;

	while ( fgets( line, sizeof line, fp ) != NULL ) {
		
		if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "#", 1 ) == 0 ) continue;

		if ( strncmp( line, "Last", 4 ) == 0 ) continue;

#define SEQFORMAT "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"
		if ( ! alphabet_read && sscanf( line, SEQFORMAT, 
			wvec[0], wvec[1], wvec[2], wvec[3], wvec[4],
			wvec[5], wvec[6], wvec[7], wvec[8], wvec[9],
			wvec[10], wvec[11], wvec[12], wvec[13], wvec[14],
			wvec[15], wvec[16], wvec[17], wvec[18], wvec[19] ) == 20 ) {

			for ( i=0;i<20;i++ ) 
				(*alphabet)[i] = wvec[i][0];
		
			i=0;

			alphabet_read = 1;

			continue;
		}

#define MFORMAT "%*i %*s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
		if ( sscanf( line, MFORMAT,
			&a, &a, &a, &a, &a, &a, &a, &a, &a, &a, 
			&a, &a, &a, &a, &a, &a, &a, &a, &a, &a ) == 20 ) {

			(*l)++;

			continue;
		}

	}

	(*alphabet)[20] = 0;

	m = fmatrix( 0, (*l)-1, 0, 19 );
	i=0;

	stream_close( fp, fc, filename );

	if ( ( fp = stream_input( filename, &fc, &ff ) ) == NULL ) {
		printf( "Error. Cannot open file %s\n", filename );
		exit( 1 );
	}

	while ( fgets( line, sizeof line, fp ) != NULL ) {
		
		if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "#", 1 ) == 0 ) continue;

		if ( strncmp( line, "Last", 4 ) == 0 ) continue;

		if ( sscanf( line, "%s", aa ) == 1 && ( aa[0] == (*alphabet)[0] ) )
			continue;
		
#define MFORMAT "%*i %*s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
		if ( sscanf( line, MFORMAT,
			&m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4],
			&m[i][5], &m[i][6], &m[i][7], &m[i][8], &m[i][9],
			&m[i][10], &m[i][11], &m[i][12], &m[i][13], &m[i][14],
			&m[i][15], &m[i][16], &m[i][17], &m[i][18], &m[i][19] ) == 20 ) {

			i++;

			continue;
		}

	}

	printf( "# Hmm matrix file L %i %s read. Alphabet %s\n", (*l), filename, (*alphabet) );
	
	stream_close( fp, fc, filename );

	return( m );
}

float	*read_bgfreq( FILENAME filename, char **alphabet )

{
	FILE	*fp;
	LINE	line;
	int	i,n;
	int	fc, ff;
	float	freq;
	char	aa;
	float	*v;
	
	if ( ( fp = stream_input( filename, &fc, &ff ) ) == NULL ) {
		printf( "Error. Cannot open file %s\n",  filename );
		exit( 1 );
	}

	(*alphabet) = cvector( 0, 19 );
	v = fvector( 0, 19 );
	i = 0;

	while ( fgets( line, sizeof line, fp ) != NULL ) {
		
		if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "#", 1 ) == 0 ) continue;

		if ( sscanf( line, "%c %f", &aa, &freq ) != 2 ) {
			printf( "Error. Wrong line format\n" );
			exit( 1 );
		}

		(*alphabet)[i] = aa;
		v[i] = freq;

		i++;
	}

	(*alphabet)[20] = 0;

	printf( "# Background frequences read. Alphabet %s\n", *alphabet );

	stream_close( fp, fc, filename );

	return( v );
}
