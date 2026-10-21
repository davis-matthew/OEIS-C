/*
 * A294452  Expansion of x*(16*x^6+20*x^4-32*x^3+24*x^2-8*x+1) /
 *          ((-1+2*x)^2*(2*x^2-4*x+1)^2).
 * https://oeis.org/A294452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 56*a(n-2) + 128*a(n-3) - 148*a(n-4) +
 *            80*a(n-5) - 16*a(n-6) (n ≥ 8) Initial values: a(0)=0, a(1)=1,
 *            a(2)=4, a(3)=16, a(4)=64, a(5)=256, a(6)=1024, a(7)=4096
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;           /* allocated size */
    static idx_t computed_upto = 0;        /* highest index computed */

    /* first 8 terms (needed for the recurrence to start at n=8) */
    static const seq_t init[8] = {
        0ULL, 1ULL, 4ULL, 16ULL, 64ULL, 256ULL, 1024ULL, 4096ULL
    };

    /* ensure we have space for at least (n+1) items */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        if (new_len < 8) new_len = 8;          /* keep 8 at least */
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) abort();

        /* copy any existing data that fits */
        idx_t old_len = cache_len;
        if (old_len > new_len) old_len = new_len;
        for (idx_t i = 0; i < old_len; ++i)
            new_cache[i] = cache ? cache[i] : 0;

        /* initialise the first 8 values from the table */
        for (idx_t i = 0; i < 8; ++i)
            new_cache[i] = init[i];

        /* free old cache */
        free(cache);
        cache = new_cache;
        cache_len = new_len;

        /* we now have the first 8 terms cached */
        computed_upto = 7;
    }

    /* compute terms for n ≥ 8 if needed */
    for (idx_t i = (computed_upto < 7 ? 8 : computed_upto + 1);
         i <= n; ++i) {
        /* recurrence: a(i) = 12*a(i-1) - 56*a(i-2) + 128*a(i-3)
                          - 148*a(i-4) + 80*a(i-5) - 16*a(i-6) */
        seq_t term = cache[i-1];
        term *= (seq_t)12;
        term -= cache[i-2] * (seq_t)56;
        term += cache[i-3] * (seq_t)128;
        term -= cache[i-4] * (seq_t)148;
        term += cache[i-5] * (seq_t)80;
        term -= cache[i-6] * (seq_t)16;
        cache[i] = term;
    }

    if (n > computed_upto)
        computed_upto = n;

    return cache[n];
}
