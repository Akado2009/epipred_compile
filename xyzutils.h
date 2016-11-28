/* M. Nielsen April 2002. mniel@cbs.dtu.dk */

/* 	XYZ Utilities
	Various Routines to make vectors, matrices and bozes
*/

typedef float THREED[3];

#define LARGEINT        9999           
#define LARGEREAL       9999.999 

#define SQR(a)          ((a)*(a))
#define PI              3.1415926535897932384626433832795028841972
#define SQRD(a,b) (((a[0]-b[0])*(a[0]-b[0]))+((a[1]-b[1])*(a[1]-b[1]))+\
        ((a[2]-b[2])*(a[2]-b[2])))
#define distance(a,b)   (sqrt(SQRD(a,b)))

#ifndef MAX
#define MAX(a,b)        ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b)        ((a)<(b)?(a):(b))
#endif

extern THREED *xvector(int l, int h);
extern void xvector_free(THREED *m, int l, int h);
extern THREED **xmatrix(int rl, int rh, int cl, int ch);
extern void xmatrix_free(THREED **m, int rl, int rh, int cl, int ch);

extern char *cvector(int l, int h);
extern void cvector_free(char *v, int l, int h);
extern char **cmatrix(int rl, int rh, int cl, int ch);
extern void cmatrix_free(char **m, int rl, int rh, int cl, int ch);

extern WORD *wvector(int l, int h);
extern void wvector_free(WORD *v, int l, int h);

extern int *ivector(int l, int h);
extern void ivector_free(int *v, int l, int h);

extern int    *ivector_init(int lo, int hi, int val);
extern int **imatrix(int rl, int rh, int cl, int ch);
extern void imatrix_free(int **v, int rl, int rh, int cl, int ch);
extern int ***ibox(int rl, int rh, int cl, int ch, int ll, int lh);
extern void ibox_free(int ***m, int rl, int rh, int cl, int ch, int ll, int lh);

extern	void    ivector_add( int *v1, int *v2, int n );
extern	int   ivector_min( int *v, int n );
extern	int   ivector_max( int *v, int n );

extern	int     *ivector_split( char *line, int *n );

extern float *fvector(int l, int h);
extern void fvector_free(float *v, int l, int h);
extern float **fmatrix(int rl, int rh, int cl, int ch);
extern void fmatrix_free(float **v, int rl, int rh, int cl, int ch);
extern float ***fbox(int rl, int rh, int cl, int ch, int ll, int lh);
extern void fbox_free(float ***m, int rl, int rh, int cl, int ch, int ll, int lh);
extern float ****fmat4(int l0, int h0, int l1, int h1, int l2, int h2, int l3, int h3);
extern void fmat4_free( float ****m, int l0, int h0, int l1, int h1, int l2, int h2, int l3, int h3);
void    fvector_add( float *v1, float *v2, int n );
void    fvector_sub( float *v1, float *v2, int n );

extern double *dvector(int l, int h);
extern void dvector_free(double *v, int l, int h);
extern double **dmatrix(int rl, int rh, int cl, int ch);
extern void dmatrix_free(double **v, int rl, int rh, int cl, int ch);
extern double ***dbox(int rl, int rh, int cl, int ch, int ll, int lh);
extern void dbox_free(double ***m, int rl, int rh, int cl, int ch, int ll, int lh);
extern double ****dmat4(int l0, int h0, int l1, int h1, int l2, int h2, int l3, int h3);
extern void dmat4_free( double ****m, int l0, int h0, int l1, int h1, int l2, int h2, int l3, int h3);

extern	int     ivector_sum( int *v, int n );

extern  int 	listfmax( float *a, int n );
extern	int 	listimax( int *v, int n );
extern	float   fvector_sum( float *v, int n );
extern	float   fvector_max( float *v, int n );
extern	float   fvector_min( float *v, int n );
extern	float   fvector_area( THREED a, THREED b, THREED c );
extern	float	fvector_dot( float *v1, float *v2, int n );
extern	float	fvector_square( float *v, int n );
extern	int     fround( float f );

extern	float	*vcrs( THREED xa, THREED xb);
extern	float	*vsub( THREED xa, THREED xb );

extern	void    ivector_rerandomize( int *v, int lo, int hi );
extern	void    fvector_rerandomize( float *v, int l, int h );
extern	void    cvector_rerandomize( char *v, int l, int h );

extern	void	xvector_cpy( THREED *x, THREED *y, int n );

extern	float	*fvector_hsort(float *v, int *o, int n, int sense);

extern	float   fvector_xycorr( int n, float *v1, float *v2 );

extern	float   fvector_xyerror( int n, float *v1, float *v2 );

extern	float	fvector_ave( float *v, int n );

extern	float   fvector_ave_u( float *v, int *u, int n);

extern	float   *fvector_split( char *line, int *n );

extern	float   gaussian(float mid, float sig);

extern	float	fvector_norm( float *v, int n );

extern	float	fvector_adiff( float *v1, float *v2, int n );

extern	float	fvector_diff( float *v1, float *v2, int n );

extern	float   *fvector_ramp( int l, int h );
extern	int   	*ivector_ramp( int l, int h );
extern	float   *fvector_unit( int l, int h );
extern	int   	*ivector_unit( int l, int h );

extern	float   cal_auc( float *v1, float *v2, int l, float thr );

extern float nearest(float a, float b);

extern float snearest(float a, float b);

extern void    fmatrix_scale(float **m, int nrl, int nrh, int ncl, int nch, float s);

extern	void    fvector_llsquare( int n, float *v1, float *v2, float *a, float *b );
