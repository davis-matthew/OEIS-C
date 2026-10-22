/*
 * A381929  Ending positions of runs in the regular paperfolding sequence
 *          A034947.
 * https://oeis.org/A381929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A381929: ending positions of runs in the regular paperfolding sequence
   (A034947).  The paperfolding value at integer n is +1 iff the odd part
   of n modulo 4 equals 1, otherwise -1.  Whenever this value changes,
   the preceding position is recorded as a term of A381929.  This
   implementation caches all results so that calls may arrive in any
   order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* reach only for n >= 1 ; offset is 1 */
    if (n == 0)
        return (seq_t)0;

    /* Cached data: array of terminating positions */
    static seq_t *ends = NULL;
    static size_t cap  = 0;   /* current allocation size */
    static size_t sz   = 0;   /* number of positions stored */
    static idx_t last_i = 0;  /* last processed index in the paperfolding sequence */
    static int last_val = 0;  /* paperfolding value at last_i */

    /* Return already computed term if available */
    if (sz >= n)
        return ends[n - 1];

    /* Initialise cache once */
    if (!ends) {
        cap  = 16;
        ends = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate run‑ending positions until we have at least n of them */
    while (sz < n) {
        /* Deal with the very first value specially */
        if (last_i == 0) {
            last_i  = 1;
            last_val = 1;          /* a(1) = +1 */
            continue;
        }

        ++last_i;                  /* move to next index */

        /* Compute paperfolding value at last_i */
        idx_t m = last_i;
        while ((m & 1U) == 0U)     /* strip powers of two */
            m >>= 1;
        int val = ((m & 3U) == 1U) ? 1 : -1;

        if (val != last_val) {     /* run ends */
            if (sz == cap) {       /* grow buffer if needed */
                cap <<= 1;
                ends = (seq_t *)realloc(ends, cap * sizeof(seq_t));
            }
            ends[sz++] = (seq_t)(last_i - 1);   /* store terminating position */
            last_val = val;                 /* update for next iteration */
        } else {
            last_val = val;                 /* keep the value for the next step */
        }
    }

    return ends[n - 1];
}
