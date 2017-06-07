#include <sys/types.h>
#include <sys/times.h>

/*********************************************************************/
/*                                                                   */
/* current processor time in seconds                                 */
/* difference between two calls is processor time spent by your code */
/* needs: <sys/types.h>, <sys/times.h>                               */
/* depends on compiler and OS                                        */
/*                                                                   */
/*********************************************************************/

float timer()
   { struct tms hold;

        times(&hold);
         return  (float)(hold.tms_utime) / 60.0;
   }
