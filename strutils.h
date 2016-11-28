/* Morten Nielsen April 2002. mniel@cbs.dtu.dk */
/* Modified from Garry Gippert source code */
/* Modified from GAP source code */

/*=
	String Utilities
	Routines to manipulate strings and regular expressions.
	Provides standard definitions for WORD, LINE, FILENAME.
=*/

#define REDIRECT_DEVNULL "/dev/null 2>&1"
#define	PROFILE_ORDER	"ARNDCQEGHILKMFPSTWYVX"

#define WORDSIZE 56
typedef char WORD[WORDSIZE];

#define FILENAMESIZE 256
typedef char FILENAME[FILENAMESIZE];

#define LINESIZE 1024
typedef char LINE[LINESIZE];

#define LONGLINESIZE 50000
typedef char LONGLINE[LONGLINESIZE];

/* routines for opening files with g(un)zip or (un)compress */

extern FILE *stream_input(char *filename, int *comp, int *fformat);
extern FILE *stream_output(char *filename, int *comp, int fformat);
extern void stream_close(FILE * fp, int comp, char *filename);

extern void skip_comments(FILE * fp);

extern int strpos(char *s, char c);

extern char *uppercase(char *s);
extern char *lowercase(char *s);

extern char **split( char *line, int *n );
extern	void    def_sepchars( char *s );

extern	char	*string_filter( char *s );

float  *gfvectorsplit(char *line, int *n);

/* end strutils.h */
