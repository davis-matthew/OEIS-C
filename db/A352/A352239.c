/*
 * A352239  Numbers k such that the decimal expansion of k and 14^k both begin
 *          with 14.
 * https://oeis.org/A352239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 34, u32 n <= 4098, u64 n <= 4098,
 *            u128 n <= 4098, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A352239: numbers k such that the decimal expansion of k and 14^k both begin with 14.
   The algorithm scans only integers whose decimal prefix is 14 and checks the
   fractional part of k*log10(14) to decide whether 14^k starts with 14.
   The values are memoised, so any order of calls is supported. */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    if (n == 0)              /* offset starts at 1 */
        return 0;

    /* ---------- static state for memoisation ---------- */
    static seq_t *cache       = NULL;  /* array of found terms   */
    static size_t cache_cap   = 0;     /* current capacity       */
    static size_t cached      = 0;     /* number of terms stored */
    static idx_t next_k       = 14;    /* next k to test         */
    static idx_t pow10_2      = 1;     /* 10^(digits-2) for block */

    /* initialise cache once */
    if (cache == NULL) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* logarithmic constants for the interval [14, 15) */
    static const double LOG14   = log10(14.0);
    static const double LOG15   = log10(15.0);
    static const double A_CONST = LOG14 - 1.0;    /* log10(14)  - 1   */
    static const double B_CONST = LOG15 - 1.0;    /* log10(15)  - 1   */

    /* generate terms until we have at least n of them */
    while (cached < n) {
        /* compute the current block: [14*10^(d-2), 15*10^(d-2)-1] */
        idx_t block_start = pow10_2 * 14;
        idx_t block_end   = pow10_2 * 15 - 1;

        if (next_k < block_start)
            next_k = block_start;

        /* iterate over the block */
        while (next_k <= block_end && cached < n) {
            /* Benford test: does 14^k start with 14? */
            double frac = (next_k * LOG14) - floor(next_k * LOG14);
            if (frac >= A_CONST && frac < B_CONST) {
                /* store the candidate */
                if (cached >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache,
                                             cache_cap * sizeof(seq_t));
                }
                cache[cached++] = (seq_t)next_k;
                if (cached == n) {       /* requested term found */
                    next_k++;            /* advance for future calls */
                    return cache[n - 1];
                }
            }
            ++next_k;
        }

        /* move to the next block of numbers that start with "14" */
        pow10_2 *= 10;
    }

    /* the requested term has just been cached */
    return cache[n - 1];
}
