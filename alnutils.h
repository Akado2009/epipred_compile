/* Morten Nielsen 21 May 2006. mniel@cbs.dtu.dk */

/* ALN database utilities */

typedef struct aln {
	struct aln *next;

	int     alen;
	WORD    type;

	WORD    qname;
	int     qlen;		/* query structure length */
	int     qof;
	char   *qal;		/* alen */
	float   qsscore;	/* q selfscore */

	WORD    dname;
	int     dlen;		/* database structure length */
	int     dof;
	char   *dal;		/* alen */
	float   dsscore;	/* d selfscore */

	int     mlen, nid, ngap;
	float   score, sscore, rscore, zscore;

} ALN;

extern void aln_verbose(int mode);

extern ALN *aln_alloc();

extern ALN *aln_read(char *filename);

extern ALN *aln_read_fp( FILE * fp);

extern void aln_free_single(ALN * a );

extern void aln_free(ALN * aln);

extern void aln_print_single(ALN * p);

extern void aln_write_single( ALN * a );

extern void aln_print(ALN * aln);

extern void aln_write_single_fp(ALN * a, FILE * fp);

extern void aln_write(ALN * aln, char *filename, int ff );

extern void aln_nid(ALN * aln);

extern float **sm_b62( FSALIST *q, FSALIST *d);

extern float ss_b62( FSALIST *q);

extern	ALN     *aln_insert( ALN *list, ALN *last, ALN *q, int type );
