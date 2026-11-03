/*
 * A315751  Coordination sequence Gal.6.628.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = 1 + 6*n – (number of 5‑step positions up to n).  
   The 5‑step positions start at indices 1,4,9,16,21,28,33,40,… 
   Each such start causes the difference a(k)–a(k–1) to be 5 instead of 6. 
*/
#include <stdint.h>

static idx_t count_5step_start(idx_t n)
{
    idx_t idx = 1;      /* first start position */
    idx_t len = 3;      /* length of the first block */
    idx_t cnt = 0;      /* number of starts seen */

    while (idx <= n) {
        ++cnt;           /* current idx is a 5‑step start   */
        idx += len;      /* move to the next block's start */

        /* after the first block, block lengths alternate 5,7,5,7,… */
        if (len == 3)
            len = 5;
        else if (len == 5)
            len = 7;
        else            /* len == 7 */
            len = 5;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t m = count_5step_start(n);
    seq_t val = (seq_t)n * 6 + 1;  /* 1 + 6n  */
    val -= (seq_t)m;              /* subtract the number of 5‑steps */
    return val;
}
