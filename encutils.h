/* M. Nielsen November 2002. mniel@cbs.dtu.dk */

/*      ENC Utilities
        Various Routines to do sequence encoding for NN
	training
*/

extern	float   **read_pscplmat( FILENAME filename, char **alphabet );

extern	float   **hmm_matrix_read( FILENAME filename, char **alphabet );

extern  float   **hmm_matrix_read_fp( FILE *fp, char **alphabet );

extern	float   **hmm_matrix_read_l( FILENAME filename, char **alphabet, int *l );

extern	float   *read_bgfreq( FILENAME filename, char **alphabet );
