/* M. Nielsen. Sept 2002. mniel@cbs.dtu.dk */

/*= 
	PEP utils 
=*/

typedef struct peplist  {
        struct 	peplist *next;
        WORD	pep;
	int	len;
	float	score;
	int	nn;
	int	*iv;
	LINE	line;
} PEPLIST;

extern	void    set_pep_verbose( int type );

extern	void    peplist_free( PEPLIST *pep );

extern	PEPLIST *peplist_alloc();

extern	PEPLIST *peplist_read( FILENAME filename );

extern	PEPLIST *peplist_read_single( FILE *fp );

extern	void    pep_print( PEPLIST *l );

extern	PEPLIST	*peplist_append( PEPLIST *list, PEPLIST *n, PEPLIST **last );

extern	PEPLIST	*peplist_insert( PEPLIST *list, PEPLIST *n );

extern  void    pep_print_fp( PEPLIST *l, FILE *fp );

extern PEPLIST **peplist_table( int n );

extern	PEPLIST *peplist_find( PEPLIST *list, char *pep );

