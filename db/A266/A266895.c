/*
 * A266895  Middle column of the "Rule 75" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A266895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the middle column of Rule 75 elementary cellular automaton 
   starting from a single ON cell (value 1).  
   The returned value is the state (0 or 1) of the central cell after n steps. 
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: initial state */
    if (n == 0) return (seq_t)1;

    /* Width of the working array.  The automaton occupies positions
       from -n-1 to +n+1 at step n, so add a guard cell on each side. */
    idx_t width = 2 * n + 3;
    idx_t center = n + 1;          /* index of position 0 */

    unsigned char *cur = (unsigned char*)malloc((size_t)width);
    unsigned char *nxt = (unsigned char*)malloc((size_t)width);
    if (!cur || !nxt) return (seq_t)0;  /* allocation failure: return 0 */

    /* initialise */
    for (idx_t i = 0; i < width; ++i) cur[i] = 0;
    cur[center] = 1;

    /* iterate the automaton for n steps */
    for (idx_t step = 0; step < n; ++step)
    {
        /* perform updates for all positions that can be reached */
        for (idx_t i = 1; i + 1 < width; ++i)
        {
            unsigned char L = cur[i - 1];
            unsigned char C = cur[i];
            unsigned char R = cur[i + 1];
            unsigned char new_val;

            /* Rule 75: output = (¬L ∧ ¬C) ∨ (¬L ∧ R) ∨ (L ∧ C ∧ ¬R) */
            if ((L == 0 && C == 0) || (L == 0 && R == 1) || (L == 1 && C == 1 && R == 0))
                new_val = 1;
            else
                new_val = 0;

            nxt[i] = new_val;
        }

        /* swap buffers */
        unsigned char *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    unsigned char result = cur[center];
    free(cur);
    free(nxt);
    return (seq_t)result;
}
