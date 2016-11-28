/* System Utilities */
/* M. Nielsen mniel@cbs.dtu.dk */
/* Modifiede version of parser made by Garry Gippert */

extern void sysinfo(int argc, char *argv[]);

#define PTYPE_SWITCH	1
#define PTYPE_INT	2
#define PTYPE_FLOAT	3
#define PTYPE_LONG	4
#define PTYPE_DOUBLE	5
#define PTYPE_CHAR	8
#define PTYPE_STRING	10
#define PTYPE_FILENAME	11
#define PTYPE_LINE  12

typedef struct parameters {
	char   *p_name;		/* parameter command line id (short form) */
	int     p_selc;		/* parameter selected */
	int     p_type;		/* parameter type */
	char   *p_addr;		/* parameter name variable address */
	char   *p_desc;		/* parameter description */
	char   *p_init;		/* parameter default value */
} PARAM;

#define	VPP	(char *)&

#define	VSWITCH	0,PTYPE_SWITCH,VPP
#define	VINT	0,PTYPE_INT,VPP
#define	VFLOAT	0,PTYPE_FLOAT,VPP
#define	VLONG	0,PTYPE_LONG,VPP
#define	VDOUBLE	0,PTYPE_DOUBLE,VPP
#define	VCHAR	0,PTYPE_CHAR,VPP
#define	VSTRING	0,PTYPE_STRING,
#define	VFILENAME 0,PTYPE_FILENAME,
#define VLINE 0,PTYPE_LINE,

#define VWORD	VSTRING
#define	VFNAME	VFILENAME

#define	TOGGLE(A)	(A)=((A)+1)%2

extern int param_env(PARAM param[]);

extern void pprint(PARAM param[]);

extern void usage(PARAM param[]);

extern void pparse(int *argc, char **argv[], PARAM param[],
	int n, char *descr);

extern void spparse(int *argc, char **argv[], PARAM param[],
	int n, char *descr);

extern char *tdate(char *date);

extern int filereadable(char *filename);

extern	void setseed(long rs);

extern char   *envfile(char *efilename, char *filename);

extern char   *xenvfile(char *str, char *tmp);

extern char   *envir(char *var, char *buf);

