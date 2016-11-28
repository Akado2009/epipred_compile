/* M. Nielsen Sept 2002. mniel@cbs.dtu.dk */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>
#include <ctype.h>

int	fsa_verbose = 1;

void    set_fsa_verbose( int type )

{
        fsa_verbose = type;
}


FSALIST	*fsalist_alloc()

{
	FSALIST	*n;

	if ( ( n = ( FSALIST * ) malloc ( sizeof( FSALIST ))) != NULL ) {
		n->next = NULL;
		n->seq = NULL;
		n->len = 0;
		n->score = -99.9;
		n->i = NULL;
		strcpy( n->name, "UNDEF" ); 
	}

	return( n );
}

#define MAXFSALEN 100000

FSALIST *fsalist_read_single( FILE *fp )

{
	FSALIST *new;
	LINE	line, junk;
	char	seq[MAXFSALEN];
	int	len, o, i, read;
	char 	ch;
	char	longline[20000];

	new = NULL;
	len = 0;
	o=0;
	seq[0] = 0;
	read = 0;

	while ( !feof( fp) && !read ) {

		if ( fp && ( ch = fgetc( fp ) ) && ungetc( ch, fp ) && ch == '#' ) {
			fgets( line, sizeof(line), fp );
			printf( "# %s", line );
			continue;
		}

		if ( fp && ( ch = fgetc( fp ) ) && ungetc( ch, fp ) && ch == '>' ) {

			if ( !new ) { /* Read name of entry */

				fgets( longline, sizeof(longline), fp );

				if ( ( new = fsalist_alloc() ) == NULL ) {
					printf( "Error. Cannot allocate FSALIST entry.\n" );
					exit( 1 );
				}

				sscanf( longline, ">%[^\n]", junk );
				strncpy( new->name, junk, sizeof( new->name ) );
				if ( strlen( junk ) >= sizeof( new->name ) )
					new->name[sizeof( new->name )-1] = 0;
			}
			else { /* Start next entry. Entry read */
				read = 1;
			}
		}
		else if ( fp && ( fgets( line, sizeof(line), fp ) != NULL ) ) {

			junk[0] = 0;

			sscanf( line, "%[^\n\t ]", junk );

			if ( strlen( junk ) < 1 ) continue;

			if ( ! isascii( junk[0] ) ) continue;

			len = strlen( junk );

			junk[len] = 0;
#ifdef DEBUG
			printf( "LINE %i %s %s\n", strlen( junk), junk, line );
#endif

			if ( o+len > MAXFSALEN ) {
				printf( "Error. %s Length > %i\n", new->name, MAXFSALEN );
				exit( 1 );
			}

			for ( i=0; i<len; i++ )
				seq[i+o] = junk[i];

			o += len;
			seq[o] = 0;

		}
	}

	if ( new ) {
		new->seq = cvector( 0, o );
		strcpy( new->seq, seq );
		new->seq[o] = 0;
		new->len = o;
	}

	return( new );

}

FSALIST	*fsalist_read( FILENAME filename )

{
	FILE	*fp;
	FSALIST	*list, *last, *new;
	int	fc, ff;
	int	n;

        if ( ( fp = stream_input( filename, &fc, &ff )) == NULL ) {
                printf( "Error. Cannot read FSALIST from file %s. Exit\n",
                        filename );
                exit( 1 );
        }

	list = NULL;
	n=0;

	while ( new = fsalist_read_single( fp ) )  {

		if ( list == NULL )
			list = new;
		else
			last->next = new;

		last = new;

		n++;
	}

	if ( fsa_verbose )
		printf( "# Number of FASTA entries read %i from file %s\n", n, filename );

	stream_close( fp, fc, filename );

	return( list );
}

FSALIST *fsalist_find( char *name, FSALIST *list, char *pattern )

{
	FSALIST	*l;
	WORD	q;
	char	*p;

	sprintf( q, pattern, name );

	for ( l=list; l; l=l->next ) {

		if ( ( p = strstr( l->name, q ) ) != NULL && 
		( p == l->name || (p-1)[0] == ' ' ) )
			return( l );

	}

	return( NULL );
}

void	fsa_print( FSALIST *l )

{
	int	len, n;
	int	i;
	char	seq[61];

	len = l->len;

	n = (int)(len/60);

	printf( ">%s\n", l->name );

	for ( i=0;i<n;i++ ) {
		strncpy( seq, l->seq + i*60, 60 );
		seq[60] = 0;
		printf( "%s\n", seq );
	}

	strcpy( seq, l->seq + n*60 );

	if ( strlen( seq ) > 0 ) 
		printf( "%s\n", seq );
}

FSALIST	*fsalist_append( FSALIST *list, FSALIST *n, FSALIST **last )

{
	FSALIST	*first;

	first = list;

	if ( first == NULL ) 
		first = n;
	else
		(*last)->next = n;

	(*last) = n;

	return( first );
}

void	fsalist_free( FSALIST *fsa )

{
	int	len;

	len = fsa->len;

	cvector_free( fsa->seq, 0, len );

	free(( char * ) fsa );
}

FSALIST	*fsalist_insert( FSALIST *list, FSALIST *new )

{
	FSALIST	*l, *o;

	if ( list == NULL )
		return( new );

	for ( l=list, o=NULL; l && l->score > new->score; o=l, l=l->next );

	if ( l == list ) { /* Insert in start of list */
		new->next = list;
		list = new;
	}
	else if ( l == NULL ) { /* Insert in end of list */
		o->next = new;
	}
	else { /* Insert after o in list */
		o->next = new;
		new->next = l;
	}

	return( list );
}

FSALIST **fsalist_table( int n )

{
	FSALIST	**new;
	int	i;

	if ( ( new = ( FSALIST ** ) malloc(( unsigned ) 
		n * sizeof( FSALIST * ) ) ) == NULL ) {
		printf( "Error. Cannot allocate FSALIST TABLE %i\n", n );
		exit( 1 );
	}

	for ( i=0;i<n;i++ )
		new[i] = NULL;

	return( new );
}

void	fsa_print_fp( FSALIST *l, FILE *fp )

{
	int	len, n;
	int	i;
	char	seq[61];

	len = l->len;

	n = (int)(len/60);

	fprintf( fp, ">%s\n", l->name );

	for ( i=0;i<n;i++ ) {
		strncpy( seq, l->seq + i*60, 60 );
		seq[60] = 0;
		fprintf( fp, "%s\n", seq );
	}

	strcpy( seq, l->seq + n*60 );
	fprintf( fp, "%s\n", seq );

}

void	fsalist_seq2upper( FSALIST *fsalist )

{
	FSALIST	*fsa;

	for ( fsa=fsalist; fsa; fsa=fsa->next ) {

		fsa->seq = uppercase( fsa->seq );

	}
}

void	fsalist_checkalpha( FSALIST *fsalist )

{
	
	FSALIST	*fsa;
	int	i, j;

	for ( fsa=fsalist; fsa; fsa=fsa->next ) {

		for ( i=0, j=0; i<fsa->len; i++ ) {
			if ( isalpha( fsa->seq[i] ) ) {
				fsa->seq[j] = fsa->seq[i];
				j++;
			}
		}
		
		fsa->seq[j] = 0;
		fsa->len = j;

	}	

}

FSALIST *fsalist_check_names( FSALIST *fsalist )

{
        FSALIST *fsa;
        int     i, l;

        for ( fsa=fsalist; fsa; fsa=fsa->next ) {

                l = strlen(fsa->name) - 1;

                while ( l >= 0 && fsa->name[l] == ' ' )
                        l--;

                l++;

                fsa->name[l] = 0;

                for ( i=0; i<l; i++ ) {
                        if ( fsa->name[i] == ' ' )
                                fsa->name[i] = '_';
                }
        }

        return( fsalist );
}

PEPLIST    *fsalist2pep( FSALIST *fsalist, int l )

{
	FSALIST *fsa;
	PEPLIST	*pep, *last, *new;
	int	i;

	pep = NULL;

        for ( fsa=fsalist; fsa; fsa=fsa->next ) {

		for ( i=0;i<fsa->len-l+1; i+=l ) {

			if ( ( new = peplist_alloc() ) == NULL ) {
				printf( "Cannot allocate PEPLIST entry. Exit\n" );
				exit( 1 );
			}

			strncpy( new->pep, fsa->seq+i, l );
			new->pep[l] = 0;
			new->len = l;
			
			if ( pep == NULL )
				pep = new;
			else
				last->next = new;

			last = new;
		}

	}

	return( pep );
}


PEPLIST    *fsalist2pep_single( FSALIST *fsa, int l, int step )

{
	PEPLIST	*pep, *last, *new;
	int	i;

	pep = NULL;

	for ( i=0;i<fsa->len-l+1; i+=step ) {

		if ( ( new = peplist_alloc() ) == NULL ) {
			printf( "Cannot allocate PEPLIST entry. Exit\n" );
			exit( 1 );
		}

		strncpy( new->pep, fsa->seq+i, l );
		new->pep[l] = 0;
		new->len = l;

		if ( pep == NULL )
			pep = new;
		else
			last->next = new;

		last = new;
	}

	return( pep );
}

void    fsalist_iassign_profile_order( FSALIST *fsalist )

{
        FSALIST    *fsa;
        int     i, n;

        for ( fsa = fsalist; fsa; fsa = fsa->next) {

                n = fsa->len;

                fsa->i = ivector(0, n - 1);

                for (i = 0; i < n; i++)
                        fsa->i[i] = strpos(PROFILE_ORDER, fsa->seq[i]);
        }
}

