/* M. Nielsen April 2002. mniel@cbs.dtu.dk */

/* 	XYZ Utilities
	Various Routines to make vectors, matrices and bozes
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <alloca.h>
#include "utils.h"

/* vector routines */

int	*ivector( int l, int h )

{
	int	i;
	int	*v;

	v = ( int * ) malloc(( unsigned ) ( h - l + 1 ) * sizeof(int));
	if ( !v ) {
		printf( "Cannot allocate ivector %i %i\n", l, h );
		exit( 1 );
	}

	v -= l;

	/* initalize vector to zero */
	for ( i=l; i<=h; i++ )
		v[i] = 0;

	return( v );
}

void	ivector_free( int *v, int l, int h )

{
	free(( char * ) ( v + l ));
}

int	ivector_sum( int *v, int n )

{
	int 	sum=0;
	int	i;
	
	for ( i=0; i<n; i++ ) 
		sum += v[i];

	return( sum );
}

void	ivector_add( int *v1, int *v2, int n )

{
	int	i;

	for ( i=0; i<n; i++ )
		v1[i] += v2[i];

}

int   ivector_min( int *v, int n )

{
	int	i;
	int   min = 999999;

	for ( i=0; i<n; i++ ) 
		if ( v[i] < min ) min = v[i];

	return( min );
}

int   ivector_max( int *v, int n )

{
	int	i;
	int   max = -999999;

	for ( i=0; i<n; i++ ) 
		if ( v[i] > max ) max = v[i];

	return( max );
}

int    *ivector_init(int lo, int hi, int val)
{
        int    *v;
        int     i;
        v = ivector(lo, hi);
        for (i = lo; i <= hi; i++)
                v[i] = val;
        return (v);
}

int   	*ivector_split( char *line, int *n )
        
{       
        char    **wvec; 
        int     i;
        int   	*v;

        wvec = split( line, n );

        v = ivector( 0, (*n)-1 );
        
        for ( i=0;i<(*n);i++ )
                v[i] = atoi( wvec[i] );
        
        for ( i=0;i<(*n);i++ )
                free( wvec[i] );
        free( wvec ); 

        return( v );
}

float	*fvector( int l, int h )

{
	int	i;
	float	*v;

	v = ( float * ) malloc(( unsigned ) ( h - l + 1 ) * sizeof(float));
	if ( !v ) {
		printf( "Cannot allocate fvector %i %i\n", l, h );
		exit( 1 );
	}

	v -= l;

	/* initalize vector to zero */
	for ( i=l; i<=h; i++ )
		v[i] = 0.0;

	return( v );
}

void	fvector_free( float *v, int l, int h )

{
	free(( char * ) ( v + l ));
}


void	fvector_add( float *v1, float *v2, int n )

{
	int	i;

	for ( i=0; i<n; i++ )
		v1[i] += v2[i];

}

void    fvector_sub( float *v1, float *v2, int n )

{
        int     i;

        for ( i=0; i<n; i++ )
                v1[i] -= v2[i];

}

float	fvector_square( float *v, int n )

{

	int	i;
	float	s;

	for ( s=0.0,i=0;i<n;i++ )
		s += v[i]*v[i];

	return( s );
}


double	*dvector( int l, int h )

{
	int	i;
	double	*v;

	v = ( double * ) malloc(( unsigned ) ( h - l + 1 ) * sizeof(double));
	if ( !v ) {
		printf( "Cannot allocate dvector %i %i\n", l, h );
		exit( 1 );
	}

	v -= l;

	/* initalize vector to zero */
	for ( i=l; i<=h; i++ )
		v[i] = 0.0;

	return( v );
}

void	dvector_free( double *v, int l, int h )

{
	free(( char * ) ( v + l ));
}

char	*cvector( int l, int h )

{
	char	*v;

	v = ( char * ) malloc(( unsigned ) ( h - l + 1 ) * sizeof(char));
	if ( !v ) {
		printf( "Cannot allocate cvector %i %i\n", l, h );
		exit( 1 );
	}

	v -= l;

	return( v );
}

void	cvector_free( char *v, int l, int h )

{
	free(( char * ) ( v + l ));
}

WORD	*wvector( int l, int h )

{
	WORD	*v;

	v = ( WORD * ) malloc(( unsigned ) ( h - l + 1 ) * sizeof( WORD ));
	if ( !v ) {
		printf( "Cannot allocate wvector %i %i\n", l, h );
		exit( 1 );
	}

	v -= l;

	return( v );
}

void	wvector_free( WORD *v, int l, int h )

{
	int	i;

	for ( i = l; i<=h; i++ )
		free( (char * ) ( v[i] ) );

	free(( char * ) ( v + l ));
}

THREED	*xvector( int l, int h )

{
	THREED	*v;

	v = ( THREED * ) malloc(( unsigned ) ( h - l + 1 ) * sizeof(THREED));
	if ( !v ) {
		printf( "Cannot allocate xvector %i %i\n", l, h );
		exit( 1 );
	}

	v -= l;

	return( v );
}

void	xvector_free( THREED *v, int l, int h )

{
	free(( char * ) ( v + l ));
}

/* Matrix routines */


int	**imatrix( int rl, int rh, int cl, int ch )

{
	int	i;
	int	**m;

	m = (int **) malloc((unsigned) ( rh - rl + 1 ) * sizeof(int * ));

	if ( ! m ) {
		printf( "Cannot allocate imatrix level 1 %i %i\n", rl, rh );
		exit( 1 );
	}

	m -= rl;

	for ( i=rl; i<= rh; i++ ) 
		m[i] = ivector( cl, ch );

	return( m );
}

void	imatrix_free( int **m, int rl, int rh, int cl, int ch )

{
	int i;
	for ( i=rh; i>= rl; i-- )
		free(( char * ) ( m[i] + cl ));
	free(( char * ) ( m + rl ));
}


float	**fmatrix( int rl, int rh, int cl, int ch )

{
	int	i;
	float	**m;

	m = (float **) malloc((unsigned) ( rh - rl + 1 ) * sizeof(float * ));

	if ( ! m ) {
		printf( "Cannot allocate fmatrix level 1 %i %i\n", rl, rh );
		exit( 1 );
	}

	m -= rl;

	for ( i=rl; i<= rh; i++ ) 
		m[i] = fvector( cl, ch );

	return( m );
}

void	fmatrix_free( float **m, int rl, int rh, int cl, int ch )

{
	int i;
	for ( i=rh; i>= rl; i-- )
		free(( char * ) ( m[i] + cl ));
	free(( char * ) ( m + rl ));
}

double	**dmatrix( int rl, int rh, int cl, int ch )

{
	int	i;
	double	**m;

	m = (double **) malloc((unsigned) ( rh - rl + 1 ) * sizeof(double * ));

	if ( ! m ) {
		printf( "Cannot allocate dmatrix level 1 %i %i\n", rl, rh );
		exit( 1 );
	}

	m -= rl;

	for ( i=rl; i<= rh; i++ ) 
		m[i] = dvector( cl, ch );

	return( m );
}

void	dmatrix_free( double **m, int rl, int rh, int cl, int ch )

{
	int i;
	for ( i=rh; i>= rl; i-- )
		free(( char * ) ( m[i] + cl ));
	free(( char * ) ( m + rl ));
}

char	**cmatrix( int rl, int rh, int cl, int ch )

{
	int	i;
	char	**m;

	m = (char **) malloc((unsigned) ( rh - rl + 1 ) * sizeof(char * ));

	if ( ! m ) {
		printf( "Cannot allocate cmatrix level 1 %i %i\n", rl, rh );
		exit( 1 );
	}

	m -= rl;

	for ( i=rl; i<= rh; i++ ) 
		m[i] = cvector( cl, ch );

	return( m );
}

void	cmatrix_free( char **m, int rl, int rh, int cl, int ch )

{
	int i;
	for ( i=rh; i>= rl; i-- )
		free(( char * ) ( m[i] + cl ));
	free(( char * ) ( m + rl ));
}

THREED	**xmatrix( int rl, int rh, int cl, int ch )

{
	int	i;
	THREED	**m;

	m = ( THREED **) malloc((unsigned) ( rh - rl + 1 ) * sizeof(THREED * ));

	if ( ! m ) {
		printf( "Cannot allocate xmatrix level 1 %i %i\n", rl, rh );
		exit( 1 );
	}

	m -= rl;

	for ( i=rl; i<= rh; i++ ) 
		m[i] = xvector( cl, ch );

	return( m );
}

void	xmatrix_free( THREED **m, int rl, int rh, int cl, int ch )

{
	int i;
	for ( i=rh; i>= rl; i-- )
		free(( char * ) ( m[i] + cl ));
	free(( char * ) ( m + rl ));
}

/* BOX routines */

int	***ibox( int rl, int rh, int cl, int ch, int ll, int lh )

{
	int	i;
	int	***m;

	m = (int *** ) malloc((unsigned) ( rh - rl + 1 ) * sizeof( int ** ));
	
	if ( ! m ) {
		printf( "Cannot allocate ibox level 1 %i %i\n", rl, rh );
		exit( 1 );
	}

	m -= rl;

	for ( i=rl; i<=rh; i++ )
		m[i] = imatrix( cl, ch, ll, lh );

	return( m );
}

void	ibox_free( int ***m, int rl, int rh, int cl, int ch, int ll, int lh )

{
	int	i, j;
	for ( i=rh; i>=rl; i-- ) {
		for ( j=ch; j>=cl; j-- ) 
			free(( char * ) (m[i][j] + ll ));
		free((char * ) ( m[i] + cl ));
	}
	free((char * ) ( m + rl ));
}

float	***fbox( int rl, int rh, int cl, int ch, int ll, int lh )

{
	int	i;
	float	***m;

	m = (float *** ) malloc((unsigned) ( rh - rl + 1 ) * sizeof( float ** ));
	
	if ( ! m ) {
		printf( "Cannot allocate fbox level 1 %i %i\n", rl, rh );
		exit( 1 );
	}

	m -= rl;

	for ( i=rl; i<=rh; i++ )
		m[i] = fmatrix( cl, ch, ll, lh );

	return( m );
}

void	fbox_free( float ***m, int rl, int rh, int cl, int ch, int ll, int lh )

{
	int	i, j;
	for ( i=rh; i>=rl; i-- ) {
		for ( j=ch; j>=cl; j-- ) 
			free(( char * ) (m[i][j] + ll ));
		free((char * ) ( m[i] + cl ));
	}
	free((char * ) ( m + rl ));
}


double	***dbox( int rl, int rh, int cl, int ch, int ll, int lh )

{
	int	i;
	double	***m;

	m = (double *** ) malloc((unsigned) ( rh - rl + 1 ) * sizeof( double ** ));
	
	if ( ! m ) {
		printf( "Cannot allocate dbox level 1 %i %i\n", rl, rh );
		exit( 1 );
	}

	m -= rl;

	for ( i=rl; i<=rh; i++ )
		m[i] = dmatrix( cl, ch, ll, lh );

	return( m );
}

void	dbox_free( double ***m, int rl, int rh, int cl, int ch, int ll, int lh )

{
	int	i, j;
	for ( i=rh; i>=rl; i-- ) {
		for ( j=ch; j>=cl; j-- ) 
			free(( char * ) (m[i][j] + ll ));
		free((char * ) ( m[i] + cl ));
	}
	free((char * ) ( m + rl ));
}

/* 4 dimension routines */

float 	****fmat4(int l0, int h0, int l1, int h1, int l2, int h2, 
	int l3, int h3)

{

	int	i;
	float	****m;

	m = (float **** ) malloc((unsigned) ( h0 - l0 + 1 ) * 
		sizeof( float ** ));
	
	if ( ! m ) {
		printf( "Cannot allocate fbox level 1 %i %i\n", l0, h0 );
		exit( 1 );
	}

	m -= l0;

	for ( i=l0; i<=h0; i++ )
		m[i] = fbox( l1, h1, l2, h2, l3, h3 );

	return( m );
}

void fmat4_free( float ****m, int l0, int h0, int l1, 
	int h1, int l2, int h2, int l3, int h3)

{

	int	i, j, k;
	for ( i=h0; i>=l0; i-- ) {
		for ( j=h1; j>=l1; j-- ) {
			for ( k=h2; k>= l2; k-- ) 
				free(( char * ) (m[i][j][k] + l3 ));
			free((char * ) ( m[i][j] + l2 ));
		}
		free(( char * ) ( m[i] + l1 ));
	}
	free((char * ) ( m + l0 ));
}

double 	****dmat4(int l0, int h0, int l1, int h1, int l2, int h2, 
	int l3, int h3)

{

	int	i;
	double	****m;

	m = (double **** ) malloc((unsigned) ( h0 - l0 + 1 ) * 
		sizeof( double ** ));
	
	if ( ! m ) {
		printf( "Cannot allocate dbox level 1 %i %i\n", l0, h0 );
		exit( 1 );
	}

	m -= l0;

	for ( i=l0; i<=h0; i++ )
		m[i] = dbox( l1, h1, l2, h2, l3, h3 );

	return( m );
}

void dmat4_free( double ****m, int l0, int h0, int l1, 
	int h1, int l2, int h2, int l3, int h3)

{

	int	i, j, k;
	for ( i=h0; i>=l0; i-- ) {
		for ( j=h1; j>=l1; j-- ) {
			for ( k=h2; k>= l2; k-- ) 
				free(( char * ) (m[i][j][k] + l3 ));
			free((char * ) ( m[i][j] + l2 ));
		}
		free(( char * ) ( m[i] + l1 ));
	}
	free((char * ) ( m + l0 ));
}

/* Routines on vectors */

float	fvector_sum( float *v, int n )

{
	float	sum=0.0;
	int	i;

	for ( i=0; i<n; i++ ) sum += v[i];

	return( sum );
}

float   fvector_max( float *v, int n )

{
	int	i;
	float   max = -999999.9;

	for ( i=0; i<n; i++ ) 
		if ( v[i] > max ) max = v[i];

	return( max );
}

float   fvector_min( float *v, int n )

{
	int	i;
	float   min = 999999.9;

	for ( i=0; i<n; i++ ) 
		if ( v[i] < min ) min = v[i];

	return( min );
}

int	fround( float f )

{
	if ( f < 0.0 ) {
		return( (int)(f - 1.0) );
	}
	else {
		return( (int)(f) );
	}
}

float	*vcrs( THREED xa, THREED xb)

{
	float	*tmp;

	tmp = fvector( 0, 2 );

        tmp[0] = xa[1] * xb[2] - xa[2] * xb[1];
        tmp[1] = xa[2] * xb[0] - xa[0] * xb[2];
        tmp[2] = xa[0] * xb[1] - xa[1] * xb[0];

	return( tmp );
}

float	*vsub( THREED xa, THREED xb )

{
	float	*tmp;

	tmp = fvector( 0, 2 );
	
	tmp[0] = xb[0] - xa[0];
	tmp[1] = xb[1] - xa[1];
	tmp[2] = xb[2] - xa[2];

	return( tmp );
}

/* RANDOM NUMBERS */

void    ivector_rerandomize( int *v, int l, int h )

{
        int i, k, t, m ;

	m = h-l+1;
        for ( i = l; i <= h; i++ ) {
                k = l + m * drand48();
                t = v[i];
                v[i] = v[k];
                v[k] = t;
        }
}

void    fvector_rerandomize( float *v, int l, int h )

{
        int 	i, k, m;
	float	t;

	m = h-l+1;
        for ( i = l; i <= h; i++ ) {
                k = l + m * drand48();
                t = v[i];
                v[i] = v[k];
                v[k] = t;
        }
}

void    cvector_rerandomize( char *v, int l, int h )

{
        int 	i, k, m;
	char	t;

	m = h-l+1;
        for ( i = l; i <= h; i++ ) {
                k = l + m * drand48();
                t = v[i];
                v[i] = v[k];
                v[k] = t;
        }
}

void	xvector_cpy( THREED *x, THREED *y, int n )

{
	int	i,j;

	if ( !x ) {
		printf( "xvector_cpy ERROR. X not difined. Exit\n" );
		exit( 1 );
	}

	if ( !y ) {
		printf( "xvector_cpy ERROR. Y not difined. Exit\n" );
		exit( 1 );
	}
	
	for ( i=0; i<n; i++ )
	for ( j=0; j<3; j++) 
		y[i][j] = x[i][j];

}

float	fvector_area( THREED a, THREED b, THREED c )

{
	THREED	ba;
	THREED	cb;
	float	area_x, area_y, area_z;
	int	i;

	for ( i=0; i<3; i++ ) {
		ba[i] = b[i] - a[i];
		cb[i] = c[i] - b[i];
	}

	area_x = ba[1]*cb[2] - ba[2]*cb[1];
	area_y = ba[2]*cb[0] - ba[0]*cb[2];
	area_z = ba[0]*cb[1] - ba[1]*cb[0];

	return( 0.5*sqrt( area_x*area_x + area_y*area_y + area_z*area_z ) );
}

float   fvector_dot(float *v1, float *v2, int n )
{
        int     i;
        float  	sum;

	sum = 0.0;

        for (i=0; i<n; i++)
                sum += v1[i] * v2[i];

        return (sum);
}


int listimax( int *v, int n )

{
	int	i, im;

	im = 0;
	for ( i=1;i<n;i++ ) {
		if ( v[i] > v[im] ) 
			im = i;
	}

	return( im );
}

int listfmax( float *v, int n )

{
	int	i, im;
#ifdef OLD
        if ( v[0] >  v[1] && v[0] >  v[2] ) return 0;
        if ( v[1] >= v[0] && v[1] >= v[2] ) return 1;
        return 2;
#else
	im = 0;
	for ( i=1;i<n;i++ )
		if ( v[i] > v[im] ) im = i;

	return( im );
#endif
#undef OLD
}

/* Heap sort routines July 2002 */

static int	ssense;

static	int	fcompare( float v1, float v2 )

{
	if ( v1 < v2 )
		return( ssense );
	else if ( v1 > v2 )
		return( -ssense );
	else
		return( 0 );
}


#define SWAP(x,y) (temp=(x), (x) = (y), (y) = temp)

static void fadjust( float *v, int *o, int m, register int n )

{
	register int *b, temp;
	int	 j, k;

	b = o-1;
	j = m;
	k = m*2;
	while (k<=n) {
		if ( k<n && fcompare( v[b[k]], v[b[k+1]] ) < 0 ) ++k;
		if ( fcompare( v[b[j]], v[b[k]] ) < 0 ) SWAP( b[j], b[k] );
		j = k;
		k *= 2;
	}
}

void heapsort_my( float *v, int *o, int n )

{
	int 	*b, temp;
	int	j;
	
	b = o-1;
	for ( j=n/2; j>0; j-- ) fadjust( v, o, j, n );
	for ( j=n-1; j>0; j-- ) {
		SWAP( b[1], b[j+1] );
		fadjust( v, o, 1, j );
	}
}

float	*fvector_hsort(float *v, int *o, int n, int sense)

{
        int     i;
	float	*fv, *fs;
	int	*fo;

	ssense = sense;

        fv = fvector(0, n - 1);
	fo = ivector(0, n - 1);
	fs = fvector(0, n - 1);

        for (i = 0; i < n; i++) {
                fv[i] = v[i];
		fo[i] = o[i];
#ifdef DEBUG
		printf( "TEST %i %f %i %i\n", i, fv[i], fo[i], o[i] );
#endif
        }

        heapsort_my(fv, fo, n);

	for (i = 0; i < n; i++) {
		fs[i] = fv[fo[i]];
		o[i] = fo[i];
	}

#ifdef DEBUG
	for ( i= 0; i < n; i++)
		printf( "HP %i %f %f %i %i\n", i, v[i], fs[i], fo[i], o[i] );
#endif

	fvector_free( fv, 0, n-1 );
	ivector_free( fo, 0, n-1 );

	return( fs );

}

float   fvector_ave( float *v, int n)

{
	int	i;
	float	a;

	a = 0.0;

	for ( i=0; i<n; i++ ) 
		a += v[i];

	return( ( n>0 ? a/n : 0.0 ) );
}

float   fvector_ave_u( float *v, int *u, int n)

{
	int	i;
	float	a;
	int	nct;

	a = 0.0;
	nct = 0;

	for ( i=0; i<n; i++ ) {

		if ( !u[i] )
			continue;

		a += v[i];
		nct++;
	}

	return( ( nct>0 ? a/nct : 0.0 ) );
}

float	fvector_xycorr( int n, float *v1, float *v2 )

{
	int	i;
	float	x0,y0;
	float	t, nx, ny;
	float	c;

	x0 = fvector_ave( v1, n );
	y0 = fvector_ave( v2, n );

	t = nx = ny = 0.0;

	for ( i=0;i<n;i++ ) {
		t += ( v1[i] - x0 ) * ( v2[i] - y0 );
		nx += ( v1[i] - x0 ) * ( v1[i] - x0 );
		ny += ( v2[i] - y0 ) * ( v2[i] - y0 );
	}

	if ( nx * ny == 0.0 )
		c = 0.0;
	else
		c = t/sqrt(nx*ny);

	return( c );
}

float   fvector_xyerror( int n, float *v1, float *v2 )

{
	int     i;
	float	err, tmp;

	err = 0.0;

	for ( i=0;i<n;i++ ) {
		tmp = v1[i] - v2[i];
		err += tmp*tmp;
	}

	err /= ( n>0 ? n : 1.0 );

	return( err );
}

float	*fvector_split( char *line, int *n )

{
	char	**wvec;
	int	i;
	float	*v;

	wvec = split( line, n );

	v = fvector( 0, (*n)-1 );

	for ( i=0;i<(*n);i++ )
		v[i] = atof( wvec[i] );

#ifdef NOTNOW
	cmatrix_free( wvec, 0, (*n)-1, 0, WORDSIZE );
#else
	for ( i=0;i<(*n);i++ )
		free( wvec[i] );
	free( wvec );
#endif

	return( v );
}

float   gaussian(float mid, float sig)
{
        float   v1, v2, r, fac, g1;

        if (sig < 0.0) {
                fprintf(stderr, "gaussian sigma %g < 0.0\n", sig);
                return (-9999.99);
        }

        r = 1.0;
        while (r >= 1.0) {
                v1 = 2.0 * drand48() - 1.0;
                v2 = 2.0 * drand48() - 1.0;
                r = v1 * v1 + v2 * v2;
        }

        fac = sqrt(-2.0 * log(r) / r);
        g1 = v1 * fac * sig + mid;

        return (g1);
}

float	fvector_norm( float *v, int n )

{
	int	i;
	float	norm;

	norm = 0;

	for ( i=0; i<n; i++ )
		norm += v[i]*v[i];

	norm = sqrt( norm );
	
	return( norm );
}

float	fvector_adiff( float *v1, float *v2, int n )

{
	int	i;
	float	diff, d;

	diff = 0;

	for ( i=0; i<n; i++ ) {
		d = v1[i] - v2[i];
		diff += sqrt( d*d );
	}

	return( diff );
}

float	fvector_diff( float *v1, float *v2, int n )

{
	int	i;
	float	diff;

	diff = 0;

	for ( i=0; i<n; i++ ) {
		diff += (v1[i] - v2[i]);
	}

	return( diff );
}

int	*ivector_unit( int l, int h )

{
	int	*v, i;

	v = ivector( l, h );

	for ( i=l; i<=h; i++ )
		v[i] = 1;

	return( v );
}

float	*fvector_unit( int l, int h )

{
	float	*v;
	int	i;

	v = fvector( l, h );

	for ( i=l; i<=h; i++ )
		v[i] = 1;

	return( v );
}


int	*ivector_ramp( int l, int h )

{
	int	*v, i;

	v = ivector( l, h );

	for ( i=l; i<=h; i++ )
		v[i] = i;

	return( v );
}


float	*fvector_ramp( int l, int h )

{
	float	*v;
	int	i;

	v = fvector( l, h );

	for ( i=l; i<=h; i++ )
		v[i] = (float)i;

	return( v );
}

float	cal_auc( float *v1, float *v2, int l, float thr )

{
	int	i,ap,an;
	float	auc, fp, tp, otp, ofr, dfr, dtp, fr;
	int	*o;
	float 	*tmp;

	ap = 0;

	o = ivector( 0, l-1 );

	for ( i=0; i<l; i++ ) {

		if ( v1[i] > thr ) 
			ap++;

		o[i] = i;

	}

	if ( ap ==0 || ap == l ) {
		printf( "# Error no AP %i\n", ap );
		return( -99.9 );
	}

	tmp = fvector_hsort( v2, o, l, 1 );

	fp = tp = otp = 0.0;
	ofr = auc = 0.0;

	for ( i=0; i<l; i++ ) {

		if ( v1[o[i]] > thr )
			tp++;
		else
			fp++;

		if ( i<l-1 && v2[o[i]] == v2[o[i+1]] )
			continue;

		fr = (float)(fp)/(l-ap);

		dfr = (fr - ofr);
		dtp = (tp + otp)*0.5/ap;

		auc += dfr * dtp;

		ofr = fr;
		otp = tp;

	}

#ifdef NOTNOW
	ivector_free( o, 0, l-1 );
#endif
	fvector_free( tmp, 0, l-1 );

	return( auc );

}

float   nearest(float a, float b)
{
        return ((int) (a / b + 0.5) * b);
}
 
/* signed nearest (symmetric about 0 ) */
float   snearest(float a, float b)
{
        return ((int) (a / b + (a > 0.0 ? 0.5 : -0.5)) * b);
}

void    fmatrix_scale(float **m, int nrl, int nrh, int ncl, int nch, float s)
{
        int     i, j;
        for (i = nrl; i <= nrh; i++)
                for (j = ncl; j <= nch; j++)
                        m[i][j] *= s;       
}

void	fvector_llsquare( int n, float *v1, float *v2, float *a, float *b )

{
	int	i;
	float	sx ,sy, sxy, sxx, t, stt;

	sx = 0.0;
	sy = 0.0;
	sxy = 0.0;
	sxx = 0.0;

	for ( i=0;i<n;i++ ) {

		sx += v1[i];
		sy += v2[i];
		sxy += v1[i]*v2[i];
		sxx += v1[i]*v2[i];
		
        }

        if ( n == 0 ) {
		*a = -99.9;
		*b = -99.9;
        }
	else {
         
        	for ( *a = 0.0,i=0; i<n; i++ ) {
                	t = v1[i] - sx/n;
                	stt += t*t;     
                	*a += t * v2[i];
        	}
         
        	*a /= stt;
        	*b = (sy - sx* (*a) )/n;

	}

}
