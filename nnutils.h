/* M. Nielsen. April 2002. mniel@cbs.dtu.dk */

/*=
	Network Input Utilities

=*/

#define MAXLAY 10

typedef struct synaps {
	struct synaps *next;
	LINE    nin, nout;
	FILENAME wgtfile;
	int     nlay, *nnlay; /* number of layers, number of neurons in layer */
	float ***wgt;
	int     maxnper;
} SYNAPS;

extern SYNAPS *synaps_alloc();

extern SYNAPS *synaps_read_file(char *filename);

extern	SYNAPS *synaps_read(char *filename);

extern	SYNAPS *synaps_read_pfix(char *filename, char *prefix);

extern	SYNAPS    *synaps_read_nfile(char *filename, int *n);

extern  SYNAPS *synaps_read_fp( FILE *fp );

/* How synaps definitions */

typedef struct synaps_how {
        struct synaps_how *next;
        LINE    nin, nout;
        FILENAME wgtfile;
        int     nlay, *nnlay; /* number of layers, number of neurons in layer */
        float ***wgt;
        int     maxnper;
	WORD	alphain, alphaout; /* input and output translation alphabet */
	int	wsize;
	int	lenpos;
        int	lsize;
        int	lcomp;
        float	pfac;
        int	iact;
        int	iprof;
	float	weight; /* weight on predictor in average */
	int	nialph;
	int	input;
} SYNAPS_HOW;

extern SYNAPS_HOW *synaps_how_alloc();

extern SYNAPS_HOW *synaps_how_read_file(char *filename);

extern	SYNAPS_HOW      *synaps_how_read(char *filename);

extern SYNAPS_HOW *synaps_how_read_file_new(char *filename);

extern SYNAPS_HOW *synaps_how_read_file_new2(char *filename);

extern  SYNAPS_HOW      *synaps_how_read_new(char *filename);

/* end nnutils.h */
