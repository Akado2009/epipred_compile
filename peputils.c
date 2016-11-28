/* M. Nielsen Sept 2002. mniel@cbs.dtu.dk */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>

int     pep_verbose = 1;

void    set_pep_verbose( int type )

{
        pep_verbose = type;
}

PEPLIST	*peplist_alloc()

{
	PEPLIST	*n;

	if ( ( n = ( PEPLIST * ) malloc ( sizeof( PEPLIST ))) != NULL ) {
		n->next = NULL;
		strcpy( n->pep, "UNDEF" ); 
		n->len = 0;
		n->score = -99.9;
		n->nn = 0;
		n->iv = NULL;
		strcpy( n->line, "UNDEF" );
	}

	return( n );
}

PEPLIST	*peplist_read( FILENAME filename )

{
	FILE	*fp;
	PEPLIST	*list, *last, *new;
	LINE	line;
	WORD	pep;
	float	sco;
	int	fc, ff;
	int	n;

        if ( ( fp = stream_input( filename, &fc, &ff )) == NULL ) {
                printf( "Error. Cannot read PEPLIST from file %s. Exit\n",
                        filename );
                exit( 1 );
        }

	list = NULL;
	n=0;

	while ( fgets(line, sizeof line, fp) != NULL) {

		if (strncmp(line, "#", 1) == 0)
                        continue;              

                if (strlen(line) <= 1)
                        continue;  

		sco = 0.0;

		if ( sscanf( line, "%s %f", pep, &sco ) < 1 ) {
			printf( "Wrong line format PEPLIST_READ_SINGLE %s\n", line );
			exit( 1 );
		}

		if ( ( new = peplist_alloc() ) == NULL ) {
			printf( "Error. Cannot allocate PEPLIST entry.\n" );
			exit( 1 );
		}

		strcpy( new->pep, pep );
		new->len = strlen( pep );
		new->score = sco;
		strncpy( new->line, line, strlen(line)-1 );
		new->line[strlen(line)] = 0;

		if ( list == NULL )
			list = new;
		else
			last->next = new;

		last = new;

		n++;
	}

	if ( pep_verbose )
		printf( "# Number of PEP entries read %i from file %s\n", n, filename );

	stream_close( fp, fc, filename );

	return( list );
}

void	pep_print( PEPLIST *l )

{

	printf( "%s %f\n", l->pep, l->score );

}

PEPLIST	*peplist_append( PEPLIST *list, PEPLIST *n, PEPLIST **last )

{
	PEPLIST	*first;

	first = list;

	if ( first == NULL ) 
		first = n;
	else
		(*last)->next = n;

	(*last) = n;

	return( first );
}

void	peplist_free( PEPLIST *pep )

{

	free(( char * ) pep );
}

PEPLIST	*peplist_insert( PEPLIST *list, PEPLIST *new )

{
	PEPLIST	*l, *o;

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

void	pep_print_fp( PEPLIST *l, FILE *fp )

{
	fprintf( fp, "%s %f\n", l->pep, l->score );

}

PEPLIST **peplist_table( int n )

{
        PEPLIST        **new;
        int             i;

        if ( ( new = ( PEPLIST ** ) malloc(( unsigned )
                n * sizeof( PEPLIST * ) ) ) == NULL ) {
                printf( "Error. Cannot allocate PEPLIST TABLE %i\n", n );
                exit( 1 );
        }
         
        for ( i=0;i<n;i++ )
                new[i] = NULL;

        return( new );
}

PEPLIST	*peplist_find( PEPLIST *list, char *pep )

{
	PEPLIST	*pl;

	for ( pl=list ;pl; pl=pl->next )
		if ( strcmp( pl->pep, pep ) == 0 )
			return( pl );

	return( NULL );
}

