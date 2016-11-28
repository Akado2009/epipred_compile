/* M. Nielsen. June 2002. mniel@cbs.dtu.dk */

/*=
        List Utilities

=*/

extern void    set_list_verbose( int type );

typedef struct namelist {
        struct namelist *next;
        WORD    name;
} NAMELIST;

extern NAMELIST *namelist_alloc();

NAMELIST        *namelist_append( NAMELIST *namelist, NAMELIST *n, 
        NAMELIST **last );

extern void namelist_free(NAMELIST * namelist);

extern NAMELIST *namelist_read(char *filename);

extern NAMELIST	*namelist_pread( char *command );

extern NAMELIST **namelist_table( int n );

extern	NAMELIST	*namelist_find( NAMELIST *namelist, char *name );

typedef struct linelist {
        struct linelist *next;
        LINE    line;
	int	nw;
	char	**wvec;
} LINELIST;

extern LINELIST *linelist_alloc();

extern LINELIST *linelist_read(char *filename);

extern	LINELIST        *linelist_append( LINELIST *linelist, LINELIST *n, LINELIST **last );

extern void linelist_free(LINELIST * linelist);

extern	LINELIST **linelist_table( int n );

extern	LINELIST        *linelist_pread( char *filename );

extern	LINELIST        *linelist_find( LINELIST *list, char *pat );

typedef struct predlist {
        struct predlist *next;
	float	*cl;
	int	ncl;
	float	tap;
	float	aff;
        WORD    id;
	WORD	pep;
	WORD	epep;
	int	isepi;
	WORD	supertype;
} PREDLIST;

extern	PREDLIST	*predlist_alloc();

extern	PREDLIST	*predlist_read( FILENAME );

extern	PREDLIST        *predlistshort_read( FILENAME );

extern	PREDLIST **predlist_table( int n );

typedef struct longlinelist {
        struct longlinelist *next;
        char   *line;
} LONGLINELIST;

extern LONGLINELIST *longlinelist_alloc();

extern LONGLINELIST *longlinelist_read(char *filename);

extern void longlinelist_free( LONGLINELIST * linelist);

extern	void    linelist_split( LINELIST *linelist );

extern	LONGLINELIST **longlinelist_table( int n );

typedef struct pairlist {
        struct pairlist *next;
        WORD	name1;
	WORD	name2;
} PAIRLIST;

extern PAIRLIST        *pairlist_alloc();

extern	PAIRLIST        *pairlist_read( char *filename );
