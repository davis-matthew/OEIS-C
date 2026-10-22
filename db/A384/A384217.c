/*
 * A384217  Consecutive states of the linear congruential pseudo-random number
 *          generator (843314861*s+453816693) mod 2^31 when started at s=1.
 * https://oeis.org/A384217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (843314861 * a(n-1) + 453816693) mod 2^31, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0;      /* number of terms currently cached */
    static size_t cap = 0;        /* allocated capacity of the cache array */

    if (n == 0) {
        return (seq_t)0;          /* undefined for offset 1, return neutral value */
    }

    /* Grow the cache if we need more terms than we have cached. */
    if (n > cached) {
        if (n > cap) {
            size_t new_cap = (cap == 0) ? 1 : cap;
            while (new_cap < (size_t)n)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();    /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }

        /* Constants for the linear congruential generator. */
        const seq_t M = (seq_t)843314861ULL;
        const seq_t C = (seq_t)453816693ULL;
        /* 2^31 mask computed using a 64‑bit shift to avoid undefined
           behaviour on small integer widths, then cast to seq_t. */
        const seq_t MODMASK = ((seq_t)((uint64_t)1 << 31)) - 1ULL;

        /* Initialise first term if necessary. */
        if (cached == 0) {
            cache[0] = (seq_t)1;   /* a(1) = 1 */
            cached = 1;
        }

        /* Compute missing terms up to n. */
        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];          /* a(i-1) */
            seq_t val  = (M * prev + C) & MODMASK;
            cache[i - 1] = val;                 /* a(i) */
        }
        cached = n;
    }

    return cache[n - 1];
}
