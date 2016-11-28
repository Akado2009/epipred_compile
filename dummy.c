/* M. Nielsen Sept 2002. mniel@cbs.dtu.dk */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "myutils.h"
#include <string.h>
#include "dummy.h"


char    *word2upper( char *word )

{
        int     i;

        for ( i=0; i<strlen(word); i++ ) {

                word[i] = toupper(word[i]);

        }
         
        return( word );
}
