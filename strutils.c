/* Morten Nielsen April 2002. mniel@cbs.dtu.dk */
/* Modified from Garry Gippert source code */
/* Modified from GAP source code */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"

/* enable reading of gzipped or compressed files */

FILE   *stream_input(char *filename, int *comp, int *fformat)
{
	FILE   *fp;
	char    buffer[2048];
	int     done;

	if (strncmp(filename, "--", 2) == 0) {
		*comp = *fformat = 0;
		return (stdin);
	}

	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Unable to open(r) file %s\n", filename);
		exit(1);
	}

	done = fgetc(fp);

	if (done == 0x1f) {
		done = fgetc(fp);
		fclose(fp);
		if (done == 0x9d) {
			*fformat = 1;
			sprintf(buffer, "uncompress -c %s 2> /dev/null\n",
				filename);
		} else if (done == 0x8b) {
			*fformat = 2;
			sprintf(buffer, "gzip -cdq %s 2> /dev/null\n",
				filename);
		} else {	/* bad magic number! */
			fprintf(stderr,
				"Unrecognised compression format %s!\n\n",
				filename);
			exit(1);
		}

		*comp = 1;
		if (!(fp = popen(buffer, "r"))) {
			fprintf(stderr,
				"Unable to decompress file >%s<!\n\n",
				filename);
			exit(1);
		}
	} else {		/* Uncompressed! */
		*fformat = 0;
		ungetc(done, fp);
		*comp = 0;
	}
	return (fp);
}

FILE   *stream_output(char *filename, int *comp, int fformat)
{
	FILE   *fp;
	char    buffer[256];
	int	l, okname;

	*comp = 0;

	switch (fformat) {

	case 0:
		if (strncmp(filename, "--", 2) == 0) {
			return (stdout);
		}
		if ((fp = fopen(filename, "w")) == NULL) {
			fprintf(stderr, "Cannot write to %s\n", filename);
			exit(1);
		}
		return (fp);
		/* break; */

	case 1:
		if (strncmp(filename, "--", 2) == 0) {
			printf("# No compression for stdout\n");
			return (stdout);
		}
#ifdef REGEXP
		sprintf(buffer, "compress -c > %s%s 2> /dev/null\n",
			filename, (G_regexp(filename, ".Z$") ? "" : ".Z"));
#else
		l = strlen( filename );
		okname = strncmp( filename+l-2, ".Z", 2 );
		sprintf(buffer, "compress -c > %s%s 2> /dev/null\n",
                        filename, (okname ? "" : ".Z"));
#endif
		break;

	case 2:
		if (strncmp(filename, "--", 2) == 0) {
			printf("# No gzip for stdout\n");
			return (stdout);
		}
#ifdef REGEXP
		sprintf(buffer, "gzip -cq > %s%s 2> /dev/null\n",
			filename, (G_regexp(filename,
					".gz$") ? "" : ".gz"));
#else
		l = strlen( filename );
		okname = strncmp( filename+l-3, ".gz", 3 );
		sprintf(buffer, "compress -c > %s%s 2> /dev/null\n",
                        filename, (okname ? "" : ".Z"));
#endif
		break;

	default:
		fprintf(stderr,
			"Unrecognised compression format %d %s!\n\n",
			fformat, filename);
		exit(1);
		break;
	}

	if (!(fp = popen(buffer, "w"))) {
		fprintf(stderr, "Unable to compress file!\n\n");
		exit(1);
	}
	*comp = 1;
	return (fp);
}

void stream_close(FILE * fp, int comp, char *filename)
{
	int     p;
	if (comp) {
/* GPG: Suddenly (September 6, 2000) pclose was returning a value of 13
   on SGIs and 36096 on Linux2 machines, even on successful popens.
   To solve this crap error, the modula 13 was put here. */
		p = pclose(fp);
		if (p != 0 && p != 13 && p != 36096) {
			fprintf(stderr,
				"Unable to close I/O process, file %s!\n\n",
				filename);
			exit(1);
		}
	} else if (fp != stdout && fp != stdin && fp != stderr)
		fclose(fp);
}

void skip_comments(FILE * fp)

{
	char    c;
	LINE	line;
	while ((c = getc(fp)) && ungetc(c, fp) && c == '#')
		fgets(line, sizeof line, fp);
}

int strpos(char *s, char c)

{
	int     i, l = strlen(s);
	for (i = 0; i < l; i++)
		if (c == s[i])
			return (i);
	return (-1);
}

char   *uppercase(char *s)
{
	int     i, l = strlen(s);
	for (i = 0; i < l; i++)
		s[i] = toupper(s[i]);
	return (s);
}

char   *lowercase(char *s)
{
	int     i, l = strlen(s);
	for (i = 0; i < l; i++)
		s[i] = tolower(s[i]);
	return (s);
}

char	*sepchars = " ,\t";

void	def_sepchars( char *s ) 

{
	sepchars = s;
}

char	**split( char *line, int *n )

{

	char	**wvec;
	WORD	word;
	int	len;
	int	e,b;

	len = strlen( line );

#ifdef DEBUG
	printf( "# Len %i string %s\n", len, line );
#endif
#undef DEBUG

	e = 0;
	(*n) = 0;
	while ( e<len ) {

		while ( e < len && strpos( sepchars, line[e] ) >= 0 )
			e++;

		b = e;
		while ( e < len && strpos( sepchars, line[e] ) < 0 )
			e++;

		if ( b < e ) {
#ifdef DEBUG
			strncpy( word, line+b, e-b );
			word[e-b] = 0;

			printf( "# Test2 b %i e %i %s.\n", 
				b,  
				e, 
				word );
#endif
#undef DEBUG
			(*n)++;
		}
	}

	wvec = cmatrix( 0, (*n)-1, 0, WORDSIZE );

	e = 0;
	(*n) = 0;
	while ( e<len ) {

		while ( e < len && strpos( sepchars, line[e] ) >= 0 )
			e++;

		b = e;
		while ( e < len && strpos( sepchars, line[e] ) < 0 )
			e++;

		if ( b < e ) {
			strncpy( wvec[(*n)], line+b, e-b );
			wvec[(*n)][e-b] = 0;

			(*n)++;
		}

#ifdef DEBUG
		printf( "# Test split n %i b %i e %i\n", 
			(*n), b, e );
#endif
	}

#ifdef DEBUG
	printf( "# Split %i token found on line %s\n", (*n), line );

	for ( e=0; e<(*n); e++ )
		printf( "TEST i %i word %s\n", e, wvec[e] ); 
#endif
#undef DEBUG

	return( wvec );
}

#undef DEBUG

#define MAXWORDS        200
static WORD words[MAXWORDS];

int gsplit(char *str, WORD * wordptr)
{
        int     len, nwords;
        len = nwords = 0;
        while (*str != '\n' && *str != '\0') {
                /* following takes care of leading white space */
                while (*str == ' ' || *str == '\t')
                        str++;
                while (*str != ' ' && *str != '\n' && *str != '\0' &&
                        *str != '\t')
                        wordptr[nwords][len++] = *str++;
                wordptr[nwords++][len] = '\0';
                len = 0;
                /* following takes care of trailing white space, (maybe) */
                while (*str == ' ' || *str == '\t')
                        str++;
        }
        if (nwords == 1 && strlen(wordptr[0]) == 0)
                nwords--;
        return (nwords);
}

float  *gfvectorsplit(char *line, int *n)
{
        int     i;
        float  *v;
        *n = gsplit(line, words);
        if (*n >= MAXWORDS) {
                fprintf(stderr, "fvectorsplit *n %d >= MAXWORDS %d\n",
                        *n, MAXWORDS);
                printf("fvectorsplit *n %d >= MAXWORDS %d\n",
                        *n, MAXWORDS);
                exit(1);
        }
        v = fvector(0, *n - 1);
        for (i = 0; i < *n; i++)
                sscanf(words[i], "%f", &v[i]);
        return (v);
}



char    *string_filter( char *s )

{
	int	i;
	int	*u;
	int	len;
	int	k;

	len = strlen( s );
	u = ivector( 0, len-1 );

	s = uppercase( s );

	for ( i=0;i<len;i++ ) {

		if ( s[i] == ' ' ) {
			u[i] = 0;
		}
		else if ( strpos( "ARNDCQEGHILKMFPSTWYV", s[i] ) < 0 ) {
			s[i] = 'X';
			u[i] = 1;
		}
		else {
			u[i] = 1;
		}
	}

	k=0;
	for ( i=0;i<len;i++ ) {
		if ( u[i] )
			s[k++] = s[i];
	}

	s[k] = 0;

	ivector_free( u, 0, len-1 );

	return( s );
}
