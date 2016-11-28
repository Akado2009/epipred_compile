/* Morten Nielsen May 2006 mniel@cbs.dtu.dk */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static int aln_verbose_mode = 0;

void aln_verbose(int mode)
{
	aln_verbose_mode = mode;
}

ALN    *aln_alloc()
{
	ALN    *p;
	if ( (p = (ALN *) malloc(sizeof(ALN)) ) != NULL) {
		p->next = NULL;

		p->alen = -99;
		strcpy(p->type, "undef");

		strcpy(p->qname, "undef");
		p->qlen = -99;
		p->qof = -99;
		p->qal = NULL;
		p->qsscore = -99.9;

		strcpy(p->dname, "undef");
		p->dlen = -99;
		p->dof = -99;
		p->dal = NULL;
		p->dsscore = -99.9;

		p->mlen = p->nid = p->ngap = -99;
		p->score = p->sscore = p->rscore = p->zscore = -99.9;
	}

	return (p);
}

ALN    *aln_read_fp( FILE * fp )

{
	ALN    *first, *last, *a;
	WORD    keyword;
	int     n, i;
	LINE	line;

	first = NULL;
	n = 0;
	a = NULL;
	while (fscanf(fp, "%s", keyword) > 0) {

		if (!strncmp(keyword, "#", 1)) {
			fscanf(fp, "%[^\n]", line);
			if (aln_verbose_mode)
				printf("## %s\n", line);
		} else if (strncmp(keyword, "ALN", 3) == 0 ) {

			if ((a = aln_alloc()) == NULL) {
				printf("Unable to allocate ALN\n");
				exit(1);
			}

			fscanf(fp,
                                "%s %d %s %d type %s alen %d %d %d %d score %g %g %g %g\n",
                                a->qname, &a->qlen,
                                a->dname, &a->dlen,
                                a->type,
                                &a->alen, &a->mlen, &a->nid, &a->ngap, 
				&a->score, &a->sscore, &a->rscore, &a->zscore );

			if ( first == NULL ) 
				first = a;
			else
				last->next = a;

			last = a;

			n++;
		}
		else if (strncmp(keyword, "QAL", 3) == 0) {
			a->qal = cvector(0, a->alen);
			fscanf(fp, "%*s %d %[^\n]", &a->qof, a->qal);
			a->qal[a->alen] = 0;
		}
		else if (strncmp(keyword, "DAL", 3) == 0) {
			a->dal = cvector(0, a->alen);
			fscanf(fp, "%*s %d %[^\n]", &a->dof, a->dal);
			a->dal[a->alen] = 0;
		}
	}

	printf("# Read %d ALN elements from file\n", n);

	return (first);
}

ALN    *aln_read(char *filename)
{
	FILE   *fp;
	int     fc, ff;
	ALN    *aln;

	printf("# ALN reading from (%s)\n", filename);

	fp = stream_input(filename, &fc, &ff);

	aln = aln_read_fp( fp );

	stream_close(fp, fc, filename);

	return( aln );
}

void aln_free_single(ALN * a)

{

	if (!a)
                return;

        if (a->qal)
                cvector_free(a->qal, 0, a->alen);

	if (a->dal)
                cvector_free(a->dal, 0, a->alen);

	free(a);
}        

void aln_free(ALN * aln)

{
	ALN    *a, *an;

	a = aln;
	while (a != NULL) {
		an = a->next;
		aln_free_single(a);
		a = an;
	}
}

void aln_write_single( ALN * a )

{
	int     i;
	WORD	shortname;

	if (!a) {
		printf( "# ALN = NULL\n");
		return;
	}
	printf( "ALN %s %d %s %d type %s alen %d %d %d %d score %g %g %g %f\n",
		a->qname, a->qlen,
		a->dname, a->dlen,
		a->type, a->alen, a->mlen, a->nid, a->ngap,
		a->score, a->sscore, a->rscore, a->zscore );

	if (a->qal) {
		if ( strlen( a->qname ) > 10 ) {
			strncpy( shortname, a->qname, 10 );
			shortname[10] = 0;
		}
		else
			strcpy( shortname, a->qname );

		printf( "QAL %s %5d %s\n", shortname, a->qof, a->qal);
	}

	if (a->dal) {
		if ( strlen( a->dname ) > 10 ) {
			strncpy( shortname, a->dname, 10 );
                	shortname[10] = 0;
		}
		else 
			strcpy( shortname, a->dname );

		printf( "DAL %s %5d %s\n", shortname, a->dof, a->dal);
	}

}

void aln_write_single_fp(ALN * a, FILE * fp)

{
	int     i;

	if (!a) {
		fprintf(fp, "# ALN = NULL\n");
		return;
	}
	fprintf(fp,
		"ALN %s %d %s %d type %s alen %d %d %d %d score %g %g %g %f\n",
		a->qname, a->qlen,
		a->dname, a->dlen,
		a->type, a->alen, a->mlen, a->nid, a->ngap,
		a->score, a->sscore, a->rscore, a->zscore );

	if (a->qal)
		fprintf(fp, "QAL %s %5d %s\n", a->qname, a->qof, a->qal);

	if (a->dal)
		fprintf(fp, "DAL %s %5d %s\n", a->dname, a->dof, a->dal);

}

void aln_write(ALN * aln, char *filename, int ff)

{
	ALN    *a;
	int     n;
	FILE   *fp;
	int     fc;

	if (!aln)
		return;

	fp = stream_output(filename, &fc, ff);

	if (!fp) {
		printf("# Cannot stream_output %s %d\n", filename, ff);
		return;
	}

	for (n = 0, a = aln; a; a = a->next, n++) {
		aln_write_single_fp(a, fp);
		fprintf(fp, "\n");
	}

	stream_close(fp, fc, filename);

	printf("# Wrote %d ALN elements to %s\n", n, filename);
}

void aln_nid(ALN * aln)

{
	ALN    *a;
	int     i;
	for (a = aln; a; a = a->next) {

		if (a->nid >= 0)
			continue;

		a->nid = 0;

		for (i = 0; i < a->alen; i++)
			a->nid += (a->qal[i] == a->dal[i]);
	}
}

float **sm_b62( FSALIST *q, FSALIST *d)

{
	float **m;
	int     i, j, qlen, dlen;
	int	iq, id;
	qlen = q->len;
	dlen = d->len;

	m = fmatrix(0, qlen - 1, 0, dlen - 1);

	for (i = 0; i < qlen; i++) {
		iq = strpos(PROFILE_ORDER, q->seq[i]);

		if ( iq < 0 ) {
			printf( "Error. Letter %c not in alphabet %s\n", 
				q->seq[i], PROFILE_ORDER );
			exit( 1 );
		}

		for (j = 0; j < dlen; j++) {
			id = strpos(PROFILE_ORDER, d->seq[j]);

			if ( id < 0 ) {
				printf( "Error. Letter %c not in alphabet %s\n", 
					d->seq[j], PROFILE_ORDER );
				exit( 1 );
			}

			m[i][j] = (float) bl_s(iq, id);
		}
	}

	return (m);
}

float ss_b62( FSALIST *p )

{
	float   s = 0.0;
	int     i, ip;

	for (i = 0; i < p->len; i++) {

		ip = strpos( PROFILE_ORDER, p->seq[i] );

		if ( ip < 0 ) {
			printf( "Error. Letter %c not in alphabet %s\n", 
				p->seq[i], PROFILE_ORDER );
			exit( 1 );
		}
		
		s += (float) bl_s( ip, ip );
	}

	return (s);
}

ALN     *aln_insert( ALN *list, ALN *last, ALN *new, int type )

{
 
        ALN     *a, *o;

        if ( list == NULL )
                return( new );

        if ( type == 1 )
                for ( a=list, o=NULL; a && a->zscore > new->zscore; o=a, a=a->next );
        else   
                for ( a=list, o=NULL; a && a->score > new->score; o=a, a=a->next );

        if ( a == list ) { /* Insert in start of list */
                new->next = list;
                list = new;      
        }    
        else if ( a == NULL ) { /* Insert in end of list */
                o->next = new;
        }    
        else { /* Insert after o in list */
                o->next = new;
                new->next = a;
        }    
             
        return( list );
}          
