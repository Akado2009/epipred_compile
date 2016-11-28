/* M. Nielsen April 2002. mniel@cbs.dtu.dk */

/*      Blosum Matrix Utilities
        Various Routines to make blosum score matrices
*/

extern	void    bl_init( int type );

extern	void    bl_init_file( FILENAME filename);

extern	void    bl_show();

extern	int	bl_s( int qi, int di );

extern	float   **read_realblosum( FILENAME filename, char **alphabet );

extern	int    **bl_init_file2( FILENAME filename, char **alphabet);

extern	float   **read_blosummat( char *filename, char **alphabet );
