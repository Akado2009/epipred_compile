/* M. Nielsen April. mniel@cbs.dtu.dk */

/*      Blosum Matrix Utilities
        Various Routines to make blosum score matrices
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "utils.h"

#define MS 21
int  **bl;

void    bl_init( int type )
{
        int     	i;
	FILE		*fp;
	FILENAME	filename;
	LINE		line;
	char		c;

        bl = imatrix(0, MS - 1, 0, MS - 1);

	sprintf( filename, "/Users/mniel/data/matrices/BLOSUM%i", 
		type );

	if ( ( fp = fopen( filename, "r" ) ) == NULL ) {
		printf( "Cannot read blosum file %s type %i\n", 
			filename ,type );
		exit( 1 );
	}

	printf( "# Reading blosum matrix from file %s\n" , filename );

	i = 0;
	while ( i< MS ) {	

		if ( fgets( line, sizeof line, fp ) == NULL ) {
			printf( "Error in blosum file reading %i\n", i );
			exit( 1 );
		}

                if ( strncmp( line, "#", 1 ) == 0 ) continue;

                if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "   A", 4 ) == 0 ) continue;

		if ( strncmp( line, "B", 1 ) == 0 ) continue;

		if ( strncmp( line, "Z", 1 ) == 0 ) continue;

#define FORMAT "%c %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i"

                sscanf( line, FORMAT, &c,
			&bl[i][0], &bl[i][1], &bl[i][2], &bl[i][3], &bl[i][4], 
			&bl[i][5], &bl[i][6], &bl[i][7], &bl[i][8], &bl[i][9],
			&bl[i][10], &bl[i][11], &bl[i][12], &bl[i][13], &bl[i][14],
			&bl[i][15], &bl[i][16], &bl[i][17], &bl[i][18], &bl[i][19], 
			&bl[i][20] );

		if ( PROFILE_ORDER[i] != c ) {
			printf( "Error in Sequence order %c %c\n",
				PROFILE_ORDER[i], c );
			exit( 1 );
		}

		i++;

        }

	if ( i != MS ) {
		printf( "Error in blosum file reading %i != %i\n", i, MS );
		exit( 1 );
	}

	fclose( fp );

        printf("# BL %i matrix initialized\n", type);
}

void    bl_init_file( FILENAME filename)

{
        int     	i;
	FILE		*fp;
	LINE		line;
	char		c;

        bl = imatrix(0, MS - 1, 0, MS - 1);

	if ( ( fp = fopen( filename, "r" ) ) == NULL ) {
		printf( "Cannot read blosum file %s\n", 
			filename );
		exit( 1 );
	}

	printf( "# Reading blosum matrix from file %s\n" , filename );

	i = 0;
	while ( i< MS ) {	

		if ( fgets( line, sizeof line, fp ) == NULL ) {
			printf( "Error in blosum file reading %i\n", i );
			exit( 1 );
		}

                if ( strncmp( line, "#", 1 ) == 0 ) continue;

                if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "   A", 4 ) == 0 ) continue;

		if ( strncmp( line, "B", 1 ) == 0 ) continue;

		if ( strncmp( line, "Z", 1 ) == 0 ) continue;

#define FORMAT "%c %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i"

                sscanf( line, FORMAT, &c,
			&bl[i][0], &bl[i][1], &bl[i][2], &bl[i][3], &bl[i][4], 
			&bl[i][5], &bl[i][6], &bl[i][7], &bl[i][8], &bl[i][9],
			&bl[i][10], &bl[i][11], &bl[i][12], &bl[i][13], &bl[i][14],
			&bl[i][15], &bl[i][16], &bl[i][17], &bl[i][18], &bl[i][19], 
			&bl[i][20] );

		if ( PROFILE_ORDER[i] != c ) {
			printf( "Error in Sequence order %c %c\n",
				PROFILE_ORDER[i], c );
			exit( 1 );
		}

		i++;

        }

	if ( i != MS ) {
		printf( "Error in blosum file reading %i != %i\n", i, MS );
		exit( 1 );
	}

	fclose( fp );

        printf("# BL %s matrix initialized\n", filename );
}

int    **bl_init_file2( FILENAME filename, char **alphabet)

{
        int     	i;
	FILE		*fp;
	LINE		line;
	char		c;
	int		**blm;

        blm = imatrix(0, 23, 0, 23);

	*alphabet = cvector( 0, 23 );

	if ( ( fp = fopen( filename, "r" ) ) == NULL ) {
		printf( "Cannot read blosum file %s\n", 
			filename );
		exit( 1 );
	}

	printf( "# Reading blosum matrix from file %s\n" , filename );

	i = 0;
	while ( i < 24 ) {	

		if ( fgets( line, sizeof line, fp ) == NULL ) {
			printf( "Error in blosum file reading %i\n", i );
			exit( 1 );
		}

                if ( strncmp( line, "#", 1 ) == 0 ) continue;

                if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "   A", 4 ) == 0 ) continue;

#define FORMAT2 "%c %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i"

                sscanf( line, FORMAT2, &c,
			&blm[i][0], &blm[i][1], &blm[i][2], &blm[i][3], &blm[i][4], 
			&blm[i][5], &blm[i][6], &blm[i][7], &blm[i][8], &blm[i][9],
			&blm[i][10], &blm[i][11], &blm[i][12], &blm[i][13], &blm[i][14],
			&blm[i][15], &blm[i][16], &blm[i][17], &blm[i][18], &blm[i][19], 
			&blm[i][20], &blm[i][21], &blm[i][22], &blm[i][23] );

		(*alphabet)[i] = c;

		i++;

        }

	fclose( fp );

        printf("# BL %s matrix initialized\n", filename );

	return( blm );
}

void    bl_show()
{
        int     i, j;
        if (bl == NULL) {
                fprintf(stderr, "Bl matrix = NULL\n");
                printf("Bl matrix = NULL\n");
                exit(1);
        }
        for (i = 0; i < MS; i++) {
                printf("%2d :", i);
                for (j = 0; j < MS; j++)
                        printf(" %i", bl[i][j]);
                printf("\n");
        }
        printf("# BL matrix printed\n");
}

int	bl_s( int qi, int di )

{

	return( bl[qi][di] );

}

float	**read_realblosum( FILENAME filename, char **alphabet )

{
	FILE	*fp;
	LINE	line;
	int	fc ,ff;
	int	l;
	float	**m;
	int	i;
	

	if ( ( fp = stream_input( filename, &fc, &ff ) ) == NULL ) {
		printf( "Cannot open file %s. Exit\n", filename );
		exit( 1 );
	}

	(*alphabet) = cvector( 0, 20 );
	m = fmatrix( 0, 19, 0 ,19 );
	l = 0;

#define MFORMAT "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"

	while ( fgets( line, sizeof line, fp ) != NULL ) {

		if ( strlen( line ) <= 1 ) continue;

		if ( strncmp( line, "#", 1 ) == 0 ) continue;

		if ( strncmp( line, "      A", 7 ) == 0 ) {

			for ( i=0;i<20;i++ ) 
				(*alphabet)[i] = line[i*7+6];

			(*alphabet)[20] = 0;

			continue;

		}

		if ( sscanf( line, MFORMAT, 
			&m[l][0], &m[l][1], &m[l][2], &m[l][3], &m[l][4],
			&m[l][5], &m[l][6], &m[l][7], &m[l][8], &m[l][9],
			&m[l][10], &m[l][11], &m[l][12], &m[l][13], &m[l][14],
			&m[l][15], &m[l][16], &m[l][17], &m[l][18], &m[l][19] ) != 20 ) {

			printf( "Wrong line format %s", line );
			exit( 1 );
		}

		l++;
	}

	printf( "# Read_realblosum done. Alphabet %s\n", *alphabet );
	
	stream_close( fp, fc, filename );

	return( m );
}

float   **read_blosummat( char *filename, char **alphabet )

{
        LINELIST        *linelist, *ln;
        float           **m;
        int             nc, l, i;
        char            **wvec;

        linelist = linelist_read( filename );

        if ( ! linelist ) {
                printf( "Error. Cannot read from file %s\n", filename );
                exit( 1 );
        }      
         
        (*alphabet) = cvector( 0, 20 );
        m = fmatrix( 0, 19, 0 ,19 );
        l = 0; 

#define MFORMAT2 "%*s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"

        for ( ln=linelist; ln; ln=ln->next ) {
               
                if ( strlen( ln->line ) <= 1 ) continue;
               
                if ( strncmp( ln->line, "#", 1 ) == 0 ) continue;
               
                if ( strncmp( ln->line, "   A", 4 ) == 0 ) {
               
                        wvec = split( ln->line, &nc );
               
                        for ( i=0;i<20;i++ )
                                (*alphabet)[i] = wvec[i][0];
               
                        (*alphabet)[20] = 0;

               
                        continue;
               
                }
               
                if ( l < 20 && sscanf( ln->line, MFORMAT2,
                        &m[l][0], &m[l][1], &m[l][2], &m[l][3], &m[l][4],
                        &m[l][5], &m[l][6], &m[l][7], &m[l][8], &m[l][9],
                        &m[l][10], &m[l][11], &m[l][12], &m[l][13], &m[l][14],
                        &m[l][15], &m[l][16], &m[l][17], &m[l][18], &m[l][19] ) != 20 ) {
               
                        printf( "Wrong line format %s", ln->line );
                        exit( 1 );
                }
               
                l++;
        }      
               
        linelist_free( linelist );
               
        printf( "# Read_realblosum done. Alphabet %s\n", *alphabet );
               
        return( m );
}              
