/*
 * A384863  Consecutive states of the linear congruential pseudo-random number
 *          generator G05CAF when started at s=1.
 * https://oeis.org/A384863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 13^13 * a(n-1) mod 2^59
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The multiplier 13^13 = 302875106592253 (fits in 59 bits). */
    const seq_t multiplier = (seq_t)302875106592253ULL;
    /* 2^59 – 1 mask for the modulo operation. */
    const seq_t mask = ((seq_t)1 << 59) - 1;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of computed terms in cache */

    if (n == 0)
        return (seq_t)0;          /* undefined, safe fallback */

    if (n > cache_len) {
        /* Increase cache to hold up to term n (index n‑1). */
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();       /* allocation failure */

        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0)
                tmp[i] = (seq_t)1;
            else
                tmp[i] = (tmp[i - 1] * multiplier) & mask;
        }
        cache = tmp;
        cache_len = n;
    }

    return cache[n - 1];
}
