/* G.P.Gippert Aug  5 1999 GGippert@strubix.dk */
/* G.P.Gippert Jan 12 2000 GGippert@strubix.dk - ANSI standard */
/* G.P.Gippert Mar 27 2001 GGippert@strubix.dk - SWXOR parameter type */
/* M. Nielsen April 2002   mniel@cbs.dtu.dk */

#include <stdio.h>
#include "utils.h"
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

time_t  time();
char   *ctime();

char   *tdate(char *date)

/* See man ctime */
{
	time_t  t;
	WORD    temp;
	t = time(NULL);
	sprintf(temp, "%s", ctime(&t));
	sscanf(temp, "%[^\n]", date);
	return (date);
}

char   *envir(char *var, char *buf)
/* See man getenv command */

{
        char   *wp;
        if ((wp = getenv(var)) == NULL)
                strcpy(buf, "unknown");
        else
                strcpy(buf, wp);
        return (buf);
}

#include <sys/utsname.h>
#include <sys/stat.h>

struct utsname host_info;
struct stat buf;

#define SYSEXIT 2

void sysinfo(int argc, char *argv[])
{
	int     i;
	LINE    user, date, pwd;

	if ((uname(&host_info)) < 0) {
		fprintf(stderr, "uname error.  Exiting...\n");
		exit(SYSEXIT);
	}
	printf("#");
	for (i = 0; i < argc; i++)
		printf(" %s", argv[i]);
	printf("\n");

	printf("# %s\n", tdate(date));
	printf("# User: %s\n", envir("USER", user));
	printf("# PWD : %s\n", envir("PWD", pwd));
	printf("# Host: %s %s %s %s\n",
		host_info.sysname,
		host_info.nodename, host_info.release, host_info.machine);
}

#define PMATCH( dp, cp ) ( strlen(dp) == strlen(cp) && strcmp(dp,cp) == 0 )

void param_init(PARAM param[])

{
	PARAM  *pp, *qq;

	if ( !param )
		return;

	/* check for conflicts in parameter naming */

	for ( pp = param; (pp + 1)->p_name != NULL; pp++ )
	for ( qq = pp + 1; qq->p_name != NULL;      qq++ ) 
		if (PMATCH(pp->p_name, qq->p_name)) {
			printf("parameter collision: %s %s\n",
				pp->p_name, qq->p_name);
			exit(0);
		}

	/* initialize parameter values */

	for ( pp = param; pp->p_name != NULL; pp++ ) {

		switch (pp->p_type) {

		case PTYPE_SWITCH:
			sscanf(pp->p_init, "%d", (int *) (pp->p_addr));
			break;

		case PTYPE_INT:
			sscanf(pp->p_init, "%d", (int *) (pp->p_addr));
			break;

		case PTYPE_FLOAT:
			sscanf(pp->p_init, "%f", (float *) (pp->p_addr));
			break;

		case PTYPE_LONG:
			sscanf(pp->p_init, "%ld", (long *) (pp->p_addr));
			break;

		case PTYPE_DOUBLE:
			sscanf(pp->p_init, "%lf", (double *) (pp->p_addr));
			break;

		case PTYPE_CHAR:
			sscanf(pp->p_init, "%c", (char *) (pp->p_addr));
			break;

		case PTYPE_STRING:
			sscanf(pp->p_init, "%s", pp->p_addr);
			break;

		case PTYPE_FILENAME:
			sscanf(pp->p_init, "%s", pp->p_addr);
			break;

		case PTYPE_LINE:
			sscanf(pp->p_init, "%s", pp->p_addr);
			break;

		default:
			printf("Parameter type undefined! %s %d %s\n",
				pp->p_name, pp->p_type, pp->p_desc);
			exit(1);
			/* break; */
		}
	}

}

int param_env(PARAM param[])

{
        PARAM  *pp;
        FILENAME filename;

        for (pp = param; pp->p_name != NULL; pp++) {

                if (pp->p_type == PTYPE_STRING || pp->p_type == PTYPE_FILENAME) {
                        strcpy(pp->p_addr, xenvfile(pp->p_addr, filename));
                }
        }        

        return (1);
}

void pprint( PARAM param[] )

{
	PARAM  *pp;
	WORD    temp;

	if ( !param )
		return;

	param_env(param);

	printf( "# Command line parameters set to:\n" );

	for ( pp = param; pp->p_name != NULL; pp++ ) {

		switch (pp->p_type) {

		case PTYPE_SWITCH:
			sprintf(temp, "[%s]", pp->p_name);
			printf("#\t%-20s %-20d %s\n",
				temp, *((int *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_INT:
			sprintf(temp, "[%s int]", pp->p_name);
			printf("#\t%-20s %-20d %s\n",
				temp, *((int *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_FLOAT:
			sprintf(temp, "[%s float]", pp->p_name);
			printf("#\t%-20s %-20f %s\n",
				temp, *((float *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_LONG:
			sprintf(temp, "[%s long]", pp->p_name);
			printf("#\t%-20s %-20ld %s\n",
				temp, *((long *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_DOUBLE:
			sprintf(temp, "[%s double]", pp->p_name);
			printf("#\t%-20s %-20lf %s\n",
				temp, *((double *) pp->p_addr),
				pp->p_desc);
			break;

		case PTYPE_CHAR:
			sprintf(temp, "[%s char]", pp->p_name);
			printf("#\t%-20s %c                    %s\n",
				temp, *((char *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_STRING:
			sprintf(temp, "[%s string]", pp->p_name);
			printf("#\t%-20s %-20s %s\n",
				temp, pp->p_addr, pp->p_desc);
			break;

		case PTYPE_FILENAME:
			sprintf(temp, "[%s filename]", pp->p_name);
			printf("#\t%-20s %-20s %s\n",
				temp, pp->p_addr, pp->p_desc);
			break;

		case PTYPE_LINE:
			sprintf(temp, "[%s line]", pp->p_name);
			printf("#\t%-20s %-20s %s\n",
				temp, pp->p_addr, pp->p_desc);
			break;
		}
	}
}

void usage( PARAM param[] )

{
	PARAM  *pp;
	WORD    temp;

	printf("\n");
	printf("\t%-20s %-20s %s\n",
		"PARAMETER", "DEFAULT VALUE", "DESCRIPTION");

	for ( pp = param; pp->p_name != NULL; pp++ ) {

		switch (pp->p_type) {

		case PTYPE_SWITCH:
			sprintf(temp, "[%s]", pp->p_name);
			printf("\t%-20s %-20d %s\n",
				temp, *((int *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_INT:
			sprintf(temp, "[%s int]", pp->p_name);
			printf("\t%-20s %-20d %s\n",
				temp, *((int *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_FLOAT:
			sprintf(temp, "[%s float]", pp->p_name);
			printf("\t%-20s %-20f %s\n",
				temp, *((float *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_LONG:
			sprintf(temp, "[%s long]", pp->p_name);
			printf("\t%-20s %-20ld %s\n",
				temp, *((long *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_DOUBLE:
			sprintf(temp, "[%s double]", pp->p_name);
			printf("\t%-20s %-20lf %s\n",
				temp, *((double *) pp->p_addr),
				pp->p_desc);
			break;

		case PTYPE_CHAR:
			sprintf(temp, "[%s char]", pp->p_name);
			printf("\t%-20s %c                    %s\n",
				temp, *((char *) pp->p_addr), pp->p_desc);
			break;

		case PTYPE_STRING:
			sprintf(temp, "[%s string]", pp->p_name);
			printf("\t%-20s %-20s %s\n",
				temp, pp->p_addr, pp->p_desc);
			break;

		case PTYPE_FILENAME:
			sprintf(temp, "[%s filename]", pp->p_name);
			printf("\t%-20s %-20s %s\n",
				temp, pp->p_addr, pp->p_desc);
			break;

		case PTYPE_LINE:
			sprintf(temp, "[%s filename]", pp->p_name);
			printf("\t%-20s %-20s %s\n",
				temp, pp->p_addr, pp->p_desc);
			break;

		}
	}
}

#define PARAMFORMAT	"%-10s%-32s %s\n"

int clparse_mn( int *argc, char **argv[], PARAM param[], int verbose )

{
	int     found;
	PARAM  *pp;
	int	narg,i, np;
	char	**tmp;
	char	**new_argv;

	/* *argc is a pointert to argc (the number of arguments on command line */
	/* **argv is a pointer to the array *argv that contains the command line
	parameters. *argv[0] is the executable **argv[0] is the first character of the
	executable */
	/* Command line options must begin with a "-" and have letters as first 
	character */

	if ( ! param )
		return (1);

	narg = 0;
	tmp = cmatrix( 0, *argc, 0, LINESIZE );
	np = *argc;

	strcpy( tmp[0], **argv );

	while ( --*argc > 0 ) {

		**argv = *++*argv;

		if ( (**argv)[0] != '-' ) {
			narg++;
			strcpy( tmp[narg], **argv);
			continue;
		}

		if (strncmp((**argv), "--", 2) == 0) {
			narg++;
			strcpy( tmp[narg], **argv);
			continue; /* stdin/stdout is used as filename */
		}

		/* break if parameter not alphabetic in position 1
		   (superset of "--" check above) */

		if (strlen((**argv)) > 1 && !(
			((**argv)[1] >= 'A' && (**argv)[1] <= 'Z') ||
			((**argv)[1] >= 'a' && (**argv)[1] <= 'z'))) {
			if (verbose)
				printf("# clparse break at %c %d\n",
					(**argv)[1], (**argv)[1]);

			break;
		}

		found = 0;

#define CAUTION "Caution, multiple occurrences of parameter %s on command line.\n"

		for ( pp = param; pp->p_name; pp++ )

			if (PMATCH(pp->p_name, (**argv))) {

				if (pp->p_selc) 
					printf( CAUTION, pp->p_name );

				switch (pp->p_type) {

				case PTYPE_SWITCH:
					TOGGLE(*((int *) pp->p_addr));
					found = 1;
					if (verbose)
						printf("# %-8s %-20d %s\n",
							pp->p_name, 
							*((int *) pp-> p_addr),
							pp->p_desc);
					break;

				case PTYPE_INT:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%d", (int *) (pp-> p_addr)) == 1);
					if (verbose)
						printf("# %-8s %-20d %s\n",
							pp->p_name, 
							*((int *) pp-> p_addr),
							pp->p_desc);
					break;

				case PTYPE_FLOAT:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%f", (float *) (pp-> p_addr)) == 1);
					if (verbose)
						printf("# %-8s %-20f %s\n",
							pp->p_name, 
							*((float *) pp-> p_addr),
							pp->p_desc);
					break;

				case PTYPE_LONG:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%ld", (long *) (pp-> p_addr)) == 1);
					if (verbose)
						printf("# %-8s %-20ld %s\n", pp->p_name, *((long *) pp->p_addr), pp->p_desc);
					break;

				case PTYPE_DOUBLE:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%lf", (double *) (pp-> p_addr)) == 1);
					if (verbose)
						printf("# %-8s %-20f %s\n",
							pp->p_name, 
							*((double *) pp-> p_addr),
							pp->p_desc);
					break;

				case PTYPE_CHAR:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%c", (char *) pp-> p_addr) == 1);
					if (verbose)
						printf("# %-8s %c                    %s\n", pp->p_name, *((char *) pp->p_addr), pp->p_desc);
					break;

				case PTYPE_STRING:

					if ( --*argc <= 0 ) {
                                                printf( "Error. PTYPE_STRING insufficient arguments for option %s\n", pp->p_name );
                                                exit( 1 );
                                        }

                                        if ( strlen( *++*argv ) >= WORDSIZE ) {
                                                printf( "Error. PTYPE_STRING parameter option %s too long. Max size %i\n", pp->p_name, WORDSIZE );
                                                exit( 1 );
                                        }

                                        found = ( sscanf((**argv), "%s", pp->p_addr) == 1);

					if (verbose)
						printf("# %-8s %-20s %s\n",
							pp->p_name,
							pp->p_addr,
							pp->p_desc);
					break;

				case PTYPE_FILENAME:

					if ( --*argc <= 0 ) {
                                                printf( "Error. PTYPE_STRING insufficient arguments for option %s\n", pp->p_name );
                                                exit( 1 );
                                        }

                                        if ( strlen( *++*argv ) >= FILENAMESIZE ) {
                                                printf( "Error. PTYPE_FILENAME parameter option %s too long. Maz size %i\n", pp->p_name, FILENAMESIZE );
                                                exit( 1 );
                                        }

                                        found = ( sscanf((**argv), "%s", pp->p_addr) == 1);

					if (verbose)
						printf("# %-8s %-20s %s\n",
							pp->p_name,
							pp->p_addr,
							pp->p_desc);
					break;

				case PTYPE_LINE:

					if ( --*argc <= 0 ) {
                                                printf( "Error. PTYPE_LINE insufficient arguments for option %s\n", pp->p_name );
                                                exit( 1 );
                                        }

                                        if ( strlen( *++*argv ) >= LINESIZE ) {
                                                printf( "Error. PTYPE_LINE parameter option %s too long. Max size %i\n", pp->p_name, LINESIZE );
                                                exit( 1 );
                                        }

                                        found = ( sscanf((**argv), "%s", pp->p_addr) == 1);

					if (verbose)
						printf("# %-8s %-20s %s\n",
							pp->p_name,
							pp->p_addr,
							pp->p_desc);
					break;

				}

				/* mark selected */
				pp->p_selc = 1;
			}

		if (!found) {
			printf("Unrecognized parameter: %s\n", (**argv));
			return (0);
		}
	}

	/* Now **argv point to the first non-parsed argument. Should
	point to the executable name! So step one back and update *argc */

	*argc = narg+1;

	new_argv = cmatrix( 0, narg, 0 , LINESIZE );

	strcpy( new_argv[0], tmp[0] );

	for ( i=1; i<=narg; i++ ) {
		strcpy(new_argv[i], tmp[i]);
	}

	cmatrix_free( tmp, 0, np, 0, LINESIZE );

	*argv = new_argv;

	return (1);
}

int clparse( int *argc, char **argv[], PARAM param[], int verbose )

{
	int     found;
	PARAM  *pp;

	/* *argc is a pointert to argc (the number of arguments on command line */
	/* **argv is a pointer to the array *argv that contains the command line
	parameters. *argv[0] is the executable **argv[0] is the first character of the
	executable */
	/* Command line options must begin with a "-" and have letters as first 
	character */

	if ( ! param )
		return (1);

	while ( --*argc > 0 && (*++*argv)[0] == '-') {

		if (strncmp((**argv), "--", 2) == 0)
			break; /* stdin/stdout is used as filename */

		/* break if parameter not alphabetic in position 1
		   (superset of "--" check above) */

		if (strlen((**argv)) > 1 && !(
			((**argv)[1] >= 'A' && (**argv)[1] <= 'Z') ||
			((**argv)[1] >= 'a' && (**argv)[1] <= 'z'))) {
			if (verbose)
				printf("# clparse break at %c %d\n",
					(**argv)[1], (**argv)[1]);

			break;
		}

		found = 0;

#define CAUTION "Caution, multiple occurrences of parameter %s on command line.\n"

#ifdef OLD
		for ( pp = param; pp->p_name != NULL; pp++ )
#else
		for ( pp = param; pp->p_name; pp++ )
#endif
			if (PMATCH(pp->p_name, (**argv))) {

				if (pp->p_selc) 
					printf( CAUTION, pp->p_name );

				switch (pp->p_type) {

				case PTYPE_SWITCH:
					TOGGLE(*((int *) pp->p_addr));
					found = 1;
					if (verbose)
						printf("# %-8s %-20d %s\n",
							pp->p_name, 
							*((int *) pp-> p_addr),
							pp->p_desc);
					break;

				case PTYPE_INT:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%d", (int *) (pp-> p_addr)) == 1);
					if (verbose)
						printf("# %-8s %-20d %s\n",
							pp->p_name, 
							*((int *) pp-> p_addr),
							pp->p_desc);
					break;

				case PTYPE_FLOAT:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%f", (float *) (pp-> p_addr)) == 1);
					if (verbose)
						printf("# %-8s %-20f %s\n",
							pp->p_name, 
							*((float *) pp-> p_addr),
							pp->p_desc);
					break;

				case PTYPE_LONG:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%ld", (long *) (pp-> p_addr)) == 1);
					if (verbose)
						printf("# %-8s %-20ld %s\n", pp->p_name, *((long *) pp->p_addr), pp->p_desc);
					break;

				case PTYPE_DOUBLE:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%lf", (double *) (pp-> p_addr)) == 1);
					if (verbose)
						printf("# %-8s %-20f %s\n",
							pp->p_name, 
							*((double *) pp-> p_addr),
							pp->p_desc);
					break;

				case PTYPE_CHAR:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%c", (char *) pp-> p_addr) == 1);
					if (verbose)
						printf("# %-8s %c                    %s\n", pp->p_name, *((char *) pp->p_addr), pp->p_desc);
					break;

				case PTYPE_STRING:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%s", pp->p_addr) == 1);
					if (verbose)
						printf("# %-8s %-20s %s\n",
							pp->p_name,
							pp->p_addr,
							pp->p_desc);
					break;

				case PTYPE_FILENAME:
					found = (--*argc > 0
						&& sscanf((*++*argv), "%s", pp->p_addr) == 1);
					if (verbose)
						printf("# %-8s %-20s %s\n",
							pp->p_name,
							pp->p_addr,
							pp->p_desc);
					break;

				}

				/* mark selected */
				pp->p_selc = 1;
			}
		if (!found) {
			printf("Unrecognized parameter: %s\n", (**argv));
			return (0);
		}
	}

	/* Now **argv point to the first non-parsed argument. Should
	point to the executable name! So step one back and update *argc */

	++*argc;
	*--*argv;

	return (1);
}

void pparse( int *argc, char **argv[], PARAM param[], int n, char *descr )

{
	char   *exec;
	exec = **argv;

	param_init(param);

	if (*argc > 1 && strcmp((*argv)[1], "-h") == 0) {
		printf("Usage: %s [-h] [args] %s\n", exec, descr);
		usage(param);
		exit(0);
	}

	sysinfo(*argc, *argv);

	if (!clparse_mn(&(*argc), &(*argv), param, 1) || *argc < n + 1) {
		printf("Usage: %s [-h] [args] %s\n", exec, descr);
		exit(1);
	}

	pprint(param);
}

void spparse( int *argc, char **argv[], PARAM param[], int n, char *descr )

{
	char   *exec;
	exec = **argv;

	param_init(param);

	if (*argc > 1 && strcmp((*argv)[1], "-h") == 0) {
		printf("Usage: %s [-h] [args] %s\n", exec, descr);
		usage(param);
		exit(0);
	}

	if (!clparse_mn(&(*argc), &(*argv), param, 0) || *argc < n + 1) {
		printf("Usage: %s [-h] [args] %s\n", exec, descr);
		exit(1);
	}

	param_env(param);

}

int filereadable( char *filename )

/* See man stat command */
{
	struct stat fstat;
	FILE   *fp;
	int     fs;

	fs = stat(filename, &fstat);

	if (fs != 0)		/* not a file */
		return (0);

	if ((fp = fopen(filename, "r")) == NULL)
		return (0);

	fclose(fp);

	return (1);
}

void setseed(long rs)

{
        long    seed;
        seed = (rs ? rs : time((time_t *) 0));
        printf("# Random seed is %ld\n", seed);
        srand48(seed);
}

char   *envfile(char *efilename, char *filename)
{
        LINE    etemp, ftemp;
        char   *rp;
        char   *getenv();
        pid_t   p, getpid();

        if (strncmp(efilename, "$", 1) == 0) {
                if (strncmp(efilename, "$PID", 4) == 0) {
#ifdef DEBUG   
                        printf("Special case PID\n");
#endif
                        p = getpid();
                        sprintf(filename, "%d", p);
                        return (filename);
                }
                strcpy(ftemp, "");
                sscanf(efilename, "%*1s%[^/]%s", etemp, ftemp);

                if ((rp = getenv(etemp)) != NULL) {

                        sprintf(filename, "%s%s", rp, ftemp);

                        return (filename);
                }
        }      
        return (efilename);
}       


char   *xenvfile(char *str, char *tmp)
/* expand $-containing sections of a filename to
   the corresponding environment variables, splitting
   on characters '.' and '/' GGippert@strubix.dk
   June 7, 2000 */
{
        int     i, j;
        LINE    env, wrd;
	char	*tmp2;
	
        strcpy(tmp, "");
        i = 0;
        while (*str != '\n' && *str != '\0') {
                while (*str != '$' && *str != '\n' && *str != '\0') {
                        tmp[i++] = *str++;
                }
                tmp[i] = 0;       
                strcpy(env, "");
                j = 0;          
                if (*str == '$')
                        env[j++] = *str++;
                while (*str != '.' && *str != '/' && *str != '$' &&
                        *str != '\n' && *str != '\0') {
                        env[j++] = *str++;
                }
                env[j] = 0;
                strcpy(wrd, "");

		tmp2 = envfile(env, wrd);
		if ( tmp2 && strlen(tmp2) > 0 )
                	strcpy(env, envfile(env, wrd));

                strcat(tmp, env);
                i += strlen(env);
        }
        tmp[i] = 0;

        return (tmp);
}      

