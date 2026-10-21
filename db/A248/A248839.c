/*
 * A248839  Number of length n arrays x(i), i=1..n with x(i) in 0..i and no
 *          value appearing more than 5 times.
 * https://oeis.org/A248839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   binom_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * DP for A248839: number of length‑n arrays x(i), i=1..n with 0≤x(i)≤i
 * and no value appearing more than 5 times.
 * We process values from n down to 1.  For each value v we decide
 * how many occurrences (k) of v to place in the among currently
 * free positions.  A free position is one that has not yet received a
 * value.  Each free position becomes available again when we move
 * to the next smaller value (v‑1).  After all values >0 have been
 * processed, the remaining free positions can only get value 0, so
 * we accept only states with at most 5 free positions.
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined, return 0 */

    size_t sz = (size_t)n + 1;
    seq_t *cur = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!cur || !nxt) {                     /* allocation failed */
        free(cur); free(nxt);
        return (seq_t)0;
    }

    /* start with the state where all positions are free, except one
       that becomes available when we introduce value n-1 after placing
       value n.  For value n there is exactly one free position. */
    cur[1] = (seq_t)1;
    size_t cur_max = 1;

    idx_t v = n;
    while (1)
    {
        /* clear next array */
        for (size_t i = 0; i <= cur_max; ++i)
            nxt[i] = (seq_t)0;
        size_t new_max = 0;

        /* each step introduces one new free position (for the next
           smaller value), except after the last iteration (v==1) */
        unsigned add = (v > 1) ? 1 : 0;

        for (size_t m = 0; m <= cur_max; ++m)
        {
            seq_t ways = cur[m];
            if (ways == (seq_t)0) continue;

            unsigned max_k = (m < 5) ? (unsigned)m : 5;
            seq_t m_val = (seq_t)m;
            for (unsigned k = 0; k <= max_k; ++k)
            {
                size_t new_m = m - k + add;
                seq_t add_ways = ways * binom_k(m_val, k);
                nxt[new_m] += add_ways;
                if (new_m > new_max) new_max = new_m;
            }
        }

        /* swap buffers */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
        cur_max = new_max;

        if (v == 1) break;            /* finished value 1, next step would be 0 */
        --v;
    }

    /* After all v>0 processed, cur[m] counts ways with m free spots.  
       These free spots will all contain 0, so accept only m<=5. */
    seq_t result = (seq_t)0;
    size_t limit = (cur_max < 5) ? cur_max : (size_t)5;
    for (size_t m = 0; m <= limit; ++m)
        result += cur[m];

    free(cur);
    free(nxt);
    return result;
}
