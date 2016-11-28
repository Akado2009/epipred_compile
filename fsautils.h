/* M. Nielsen. Sept 2002. mniel@cbs.dtu.dk */

/*= 
	Fasta utils 
=*/


typedef struct fsalist  {
        struct 	fsalist *next;
        char   	*seq;
        char   	name[255];
	int	len;
	float	score;
	int	*i;
} FSALIST;

extern	void    set_fsa_verbose( int type );

extern	void    fsalist_free( FSALIST *fsa );

extern	FSALIST *fsalist_alloc();

extern	FSALIST *fsalist_read( FILENAME filename );

extern	FSALIST *fsalist_read_single( FILE *fp );

extern	FSALIST *fsalist_find( char *name, FSALIST *list, char *pattern );

extern	void    fsa_print( FSALIST *l );

extern	FSALIST	*fsalist_append( FSALIST *list, FSALIST *n, FSALIST **last );

extern	FSALIST	*fsalist_insert( FSALIST *list, FSALIST *n );

extern	FSALIST **fsalist_table( int n );

extern  void    fsa_print_fp( FSALIST *l, FILE *fp );

extern	void    fsalist_seq2upper( FSALIST *fsalist );

extern	void    fsalist_checkalpha( FSALIST *fsalist );

extern FSALIST *fsalist_check_names( FSALIST *fsalist );

extern	PEPLIST    *fsalist2pep( FSALIST *fsalist, int l );

extern	PEPLIST    *fsalist2pep_single( FSALIST *fsa, int l, int step );

extern	void    fsalist_iassign_profile_order( FSALIST *fsalist );
