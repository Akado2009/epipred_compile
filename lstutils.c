/* M. Nielsen June 2002. mniel@cbs.dtu.dk */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int	list_verbose=1;

void	set_list_verbose( int type )

{
	list_verbose = type;

}

NAMELIST 	*namelist_alloc()

{
	NAMELIST	*n;

	if ( ( n = ( NAMELIST * ) malloc ( sizeof(NAMELIST))) != NULL ) {
		strcpy( n->name, "UNDEF" );
		n->next = NULL;
	}

	return( n );
}

NAMELIST	*namelist_read( char *filename )

{
	NAMELIST	*first, *last, *new;
	FILE		*fp;
	int		ff, fc;
	WORD		name;
	LINE		line;
	int		n;

	first = NULL;
	n = 0;

	if ( ( fp = stream_input( filename, &fc, &ff )) == NULL ) {
		printf( "Error. Cannot read NAMELIST from file %s. Exit\n",
			filename );
		exit( 1 );
	}

	while ( fgets(line, sizeof line, fp) != NULL ) {

		if ( strncmp( line, "#", 1 ) == 0 )
			continue;

		if ( strlen( line ) < 1 ) 
			continue;

		if ( sscanf( line, "%s", name ) != 1 )
			continue;

		if ( ( new = namelist_alloc()) == NULL ) {
			printf( "Error. Cannot allocate namelist. Exit\n" );
			exit( 1 );
		}

		strcpy( new->name, name );

		if ( first == NULL )
			first = new;
		else
			last->next = new;

		last = new;
		n++;
	}

	stream_close( fp, fc, filename );

	printf( "# Read %i elements on namelist %s\n", n, filename );

	return( first );

}

NAMELIST	*namelist_pread( char *filename )

{
	NAMELIST	*first, *last, *new;
	FILE		*pp;
	WORD		name;
	LINE		line;
	int		n;

	first = NULL;
	n = 0;

	if ( ( pp = popen( filename, "r" )) == NULL ) {
		printf( "Error. Cannot read NAMELIST from process %s. Exit\n",
			filename );
		exit( 1 );
	}

	while ( fgets(line, sizeof line, pp) != NULL ) {

		if ( strncmp( line, "#", 1 ) == 0 )
			continue;

		if ( strlen( line ) < 1 ) 
			continue;

		if ( sscanf( line, "%s", name ) != 1 )
			continue;

		if ( ( new = namelist_alloc()) == NULL ) {
			printf( "Error. Cannot allocate namelist. Exit\n" );
			exit( 1 );
		}

		strcpy( new->name, name );

		if ( first == NULL )
			first = new;
		else
			last->next = new;

		last = new;
		n++;
	}

	pclose( pp );

	printf( "# Read %i elements on namelist %s\n", n, filename );

	return( first );

}

NAMELIST	*namelist_append( NAMELIST *namelist, NAMELIST *n, 
	NAMELIST **last )

{
	NAMELIST *first;

	first = namelist;

	if ( first == NULL )
		first = n;
	else
		(*last)->next = n;

	(*last) = n;

	return( first );
}

void	namelist_free( NAMELIST *namelist )

{
	NAMELIST	*l, *n;

	for ( l=namelist; l; l=n ) {
		n=l->next;
		free( l );
	}
}

NAMELIST **namelist_table( int n )

{
	NAMELIST	**new;
	int		i;

	if ( ( new = ( NAMELIST ** ) malloc(( unsigned ) 
		n * sizeof( NAMELIST * ) ) ) == NULL ) {
		printf( "Error. Cannot allocate NAMELIST TABLE %i\n", n );
		exit( 1 );
	}

	for ( i=0;i<n;i++ )
		new[i] = NULL;

	return( new );
}

NAMELIST        *namelist_find( NAMELIST *namelist, char *name )

{
	NAMELIST	*nl;

	for ( nl=namelist; nl; nl=nl->next ) {

		if ( strcmp( nl->name, name ) == 0 )
			return( nl );
	}

	return( NULL );
}

LINELIST 	*linelist_alloc()

{
	LINELIST	*n;

	if ( ( n = ( LINELIST * ) malloc ( sizeof( LINELIST ))) != NULL ) {
		strcpy( n->line, "UNDEF" );
		n->wvec = NULL;
		n->nw = 0;
		n->next = NULL;
	}

	return( n );
}

LINELIST	*linelist_read( char *filename )

{
	LINELIST	*first, *last, *new;
	FILE		*fp;
	int		ff, fc;
	LINE		line, text;
	int		n;

	first = NULL;
	n = 0;

	if ( ( fp = stream_input( filename, &fc, &ff )) == NULL ) {
		printf( "Error. Cannot read NAMELIST from file %s. Exit\n",
			filename );
		exit( 1 );
	}

	while ( fgets(line, sizeof line, fp) != NULL ) {

		if ( strncmp( line, "#", 1 ) == 0 )
			continue;

		if ( strlen( line ) < 1 ) 
			continue;

		if ( sscanf( line, "%[^\n]", text ) != 1 )
			continue;

		if ( ( new = linelist_alloc()) == NULL ) {
			printf( "Error. Cannot allocate linelist. Exit\n" );
			exit( 1 );
		}

		strcpy( new->line, text );

		if ( first == NULL )
			first = new;
		else
			last->next = new;

		last = new;
		n++;
	}

	stream_close( fp, fc, filename );

	if ( list_verbose )
		printf( "# Read %i elements on linelist %s\n", n, filename );

	return( first );

}

void	linelist_free( LINELIST *linelist )

{
	LINELIST	*l, *n;

	for ( l=linelist; l; l=n ) {
		n=l->next;
		free( l );
	}
}

LINELIST	*linelist_append( LINELIST *linelist, LINELIST *n, 
	LINELIST **last )

{
	LINELIST *first;

	first = linelist;

	if ( first == NULL )
		first = n;
	else
		(*last)->next = n;

	(*last) = n;

	return( first );
}

LINELIST **linelist_table( int n )

{
	LINELIST	**new;
	int		i;

	if ( ( new = ( LINELIST ** ) malloc(( unsigned ) 
		n * sizeof( LINELIST * ) ) ) == NULL ) {
		printf( "Error. Cannot allocate LINELIST TABLE %i\n", n );
		exit( 1 );
	}

	for ( i=0;i<n;i++ )
		new[i] = NULL;

	return( new );
}

LINELIST	*linelist_pread( char *filename )

{
	LINELIST	*first, *last, *new;
	FILE		*pp;
	LINE		line, text;
	int		n;

	first = NULL;
	n = 0;

	if ( ( pp = popen( filename, "r" )) == NULL ) {
		printf( "Error. Cannot read NAMELIST from process %s. Exit\n",
			filename );
		exit( 1 );
	}

	while ( fgets(line, sizeof line, pp) != NULL ) {

		if ( strncmp( line, "#", 1 ) == 0 )
			continue;

		if ( strlen( line ) < 1 ) 
			continue;

		if ( sscanf( line, "%[^\n]", text ) != 1 )
			continue;

		if ( ( new = linelist_alloc()) == NULL ) {
			printf( "Error. Cannot allocate linelist. Exit\n" );
			exit( 1 );
		}

		strcpy( new->line, text );

		if ( first == NULL )
			first = new;
		else
			last->next = new;

		last = new;
		n++;
	}

	pclose( pp );

	if ( list_verbose )
		printf( "# Read %i elements on linelist %s\n", n, filename );

	return( first );

}

LINELIST	*linelist_find( LINELIST *list, char *pat )

{
	LINELIST *ll;

	for ( ll=list; ll; ll=ll->next ) {
		if ( strstr( ll->line, pat ) != NULL )
			return( ll );
	}

	return( NULL );
}

PREDLIST        *predlist_alloc()

{
        PREDLIST        *n;

        if ( ( n = ( PREDLIST * ) malloc ( sizeof(PREDLIST))) != NULL ) {
		n->cl = NULL;
		n->ncl = -9;
		n->tap = -999.9;
		n->aff = -999.9;
                strcpy( n->id, "UNDEF" );
		strcpy( n->pep, "UNDEF" );
		strcpy( n->epep, "UNDEF" );
		n->isepi = 0;
                n->next = NULL;
		strcpy( n->supertype, "UNDEF" );
        }
         
        return( n );
}

PREDLIST	*predlist_read( FILENAME	filename )

{
	PREDLIST        *first, *last, *new;
        FILE            *fp;
        int             ff, fc;
        LINE            line;
        int             n;
	int		i;
	float		*cl, tap, aff;
	WORD		id, pep, epep;
	int		isepi, nc;
	WORD		sp;

        first = NULL;
        n = 0;
              
        if ( ( fp = stream_input( filename, &fc, &ff )) == NULL ) {
                printf( "Error. Cannot read PREDLIST from file %s. Exit\n",
                        filename );
                exit( 1 );
        }

	cl = fvector( 0, 8 );

	while ( fgets(line, sizeof line, fp) != NULL ) {
       
                if ( strncmp( line, "#", 1 ) == 0 )
                        continue;
       
                if ( strlen( line ) < 1 )
                        continue;

		strcpy( sp, "UNDEF" );

		if ( 
			( ( nc = sscanf( line, 
				"ID %s pep %s epep %s aff %f cle %f %f %f %f %f %f %f %f %f tap %f ISEPI %i",
                        	id, pep, epep, &aff,
                        	&(cl[0]), &(cl[1]), &(cl[2]), &(cl[3]), &(cl[4]),
                        	&(cl[5]), &(cl[6]), &(cl[7]), &(cl[8]), &tap, &isepi ) ) ) != 15 &&
			( ( nc = sscanf( line, 
				"ID %s pep %s epep %s aff %f cle %f %f %f %f %f %f %f %f %f tap %f ISEPI %i %s",
                        	id, pep, epep, &aff,
                        	&(cl[0]), &(cl[1]), &(cl[2]), &(cl[3]), &(cl[4]),
                        	&(cl[5]), &(cl[6]), &(cl[7]), &(cl[8]), &tap, &isepi, sp ) ) ) != 16 ) {

#ifdef DEBUG
				printf( "Error. Wrong line format %i %s\n", nc, line );
				exit( 1 );
#else
				continue;
#endif

			}

                if ( ( new = predlist_alloc()) == NULL ) {
                        printf( "Error. Cannot allocate predlist. Exit\n" );
                        exit( 1 );
                }
       
                strcpy( new->id, id );
		strcpy( new->pep, pep );
		strcpy( new->epep, epep );
		new->ncl = 9;
		new->cl = fvector( 0, 8 );
		for ( i=0; i<new->ncl; i++ )
			new->cl[i] = cl[i];
		new->tap = tap;
		new->aff = aff;
		new->isepi = isepi;
		strcpy( new->supertype, sp );
       
                if ( first == NULL )
                        first = new;
                else
                        last->next = new;
       
                last = new;
                n++;
        }
       
        stream_close( fp, fc, filename );
       
        printf( "# Read %i elements on predlist %s\n", n, filename );
       
        return( first );
       
}      

PREDLIST	*predlistshort_read( FILENAME	filename )

{
	PREDLIST        *first, *last, *new;
        FILE            *fp;
        int             ff, fc;
        LINE            line;
        int             n;
	int		i;
	float		*cl, tap, aff;
	WORD		id, pep, epep;
	int		isepi;
	WORD		sp;

        first = NULL;
        n = 0;
              
        if ( ( fp = stream_input( filename, &fc, &ff )) == NULL ) {
                printf( "Error. Cannot read PREDLIST from file %s. Exit\n",
                        filename );
                exit( 1 );
        }

	cl = fvector( 0, 0 );

	while ( fgets(line, sizeof line, fp) != NULL ) {
       
                if ( strncmp( line, "#", 1 ) == 0 )
                        continue;
       
                if ( strlen( line ) < 1 )
                        continue;

		strcpy( sp, "UNDEF" );

		if ( sscanf( line, 
				"ID %s pep %s epep %s aff %f cle %f tap %f ISEPI %i",
                        	id, pep, epep, &aff,
                        	&(cl[0]), &tap, &isepi ) != 7 )
			continue;

                if ( ( new = predlist_alloc()) == NULL ) {
                        printf( "Error. Cannot allocate predlist. Exit\n" );
                        exit( 1 );
                }
       
                strcpy( new->id, id );
		strcpy( new->pep, pep );
		strcpy( new->epep, epep );
		new->ncl = 1;
		new->cl = fvector( 0, 0 );
		for ( i=0; i<new->ncl; i++ )
			new->cl[i] = cl[i];
		new->tap = tap;
		new->aff = aff;
		new->isepi = isepi;
		strcpy( new->supertype, sp );
       
                if ( first == NULL )
                        first = new;
                else
                        last->next = new;
       
                last = new;
                n++;
        }
       
        stream_close( fp, fc, filename );
       
        printf( "# Read %i elements on predlist %s\n", n, filename );
       
        return( first );
       
}      

PREDLIST **predlist_table( int n )

{
	PREDLIST	**new;
	int		i;

	if ( ( new = ( PREDLIST ** ) malloc(( unsigned ) 
		n * sizeof( PREDLIST * ) ) ) == NULL ) {
		printf( "Error. Cannot allocate PREDLIST TABLE %i\n", n );
		exit( 1 );
	}

	for ( i=0;i<n;i++ )
		new[i] = NULL;

	return( new );
}

LONGLINELIST 	*longlinelist_alloc()

{
	LONGLINELIST	*n;

	if ( ( n = ( LONGLINELIST * ) malloc ( sizeof( LONGLINELIST ))) != NULL ) {
		n->next = NULL;
		n->line = NULL;
	}

	return( n );
}

LONGLINELIST	*longlinelist_read( char *filename )

{
	LONGLINELIST	*first, *last, *new;
	FILE		*fp;
	int		ff, fc;
	LONGLINE		line, text;
	int		n;

	first = NULL;
	n = 0;

	if ( ( fp = stream_input( filename, &fc, &ff )) == NULL ) {
		printf( "Error. Cannot read NAMELIST from file %s. Exit\n",
			filename );
		exit( 1 );
	}

	while ( fgets(line, sizeof line, fp) != NULL ) {

		if ( strncmp( line, "#", 1 ) == 0 )
			continue;

		if ( strlen( line ) < 1 ) 
			continue;

		if ( sscanf( line, "%[^\n]", text ) != 1 )
			continue;

		if ( ( new = longlinelist_alloc()) == NULL ) {
			printf( "Error. Cannot allocate longlinelist. Exit\n" );
			exit( 1 );
		}

		new->line = cvector( 0, strlen( text ) );

		strcpy( new->line, text );

		if ( first == NULL )
			first = new;
		else
			last->next = new;

		last = new;
		n++;
	}

	stream_close( fp, fc, filename );

	printf( "# Read %i elements on longlinelist %s\n", n, filename );

	return( first );

}

void	longlinelist_free( LONGLINELIST *linelist )

{
	LONGLINELIST	*l, *n;

	for ( l=linelist; l; l=n ) {
		n=l->next;
		cvector_free( l->line, 0, 999 );
		free( l );
	}
}

void	linelist_split( LINELIST *linelist )

{
	LINELIST	*ll;

	for ( ll =linelist; ll; ll=ll->next )
		ll->wvec = split( ll->line, &(ll->nw) );

}

LONGLINELIST **longlinelist_table( int n )

{
	LONGLINELIST	**new;
	int		i;

	if ( ( new = ( LONGLINELIST ** ) malloc(( unsigned ) 
		n * sizeof( LONGLINELIST * ) ) ) == NULL ) {
		printf( "Error. Cannot allocate LONGLINELIST TABLE %i\n", n );
		exit( 1 );
	}

	for ( i=0;i<n;i++ )
		new[i] = NULL;

	return( new );
}

PAIRLIST 	*pairlist_alloc()

{
	PAIRLIST	*n;

	if ( ( n = ( PAIRLIST * ) malloc ( sizeof(PAIRLIST))) != NULL ) {
		strcpy( n->name1, "UNDEF" );
		strcpy( n->name2, "UNDEF" );
		n->next = NULL;
	}

	return( n );
}

PAIRLIST	*pairlist_read( char *filename )

{
	PAIRLIST	*first, *last, *new;
	FILE		*fp;
	int		ff, fc;
	WORD		name1, name2;
	LINE		line;
	int		n;

	first = NULL;
	n = 0;

	if ( ( fp = stream_input( filename, &fc, &ff )) == NULL ) {
		printf( "Error. Cannot read PAIRLIST from file %s. Exit\n",
			filename );
		exit( 1 );
	}

	while ( fgets(line, sizeof line, fp) != NULL ) {

		if ( strncmp( line, "#", 1 ) == 0 )
			continue;

		if ( strlen( line ) < 1 ) 
			continue;

		if ( sscanf( line, "%s %s", name1, name2 ) != 2 )
			continue;

		if ( ( new = pairlist_alloc()) == NULL ) {
			printf( "Error. Cannot allocate pairlist. Exit\n" );
			exit( 1 );
		}

		strcpy( new->name1, name1 );
		strcpy( new->name2, name2 );	

		if ( first == NULL )
			first = new;
		else
			last->next = new;

		last = new;
		n++;
	}

	stream_close( fp, fc, filename );

	printf( "# Read %i elements on pairlist %s\n", n, filename );

	return( first );

}
