/*
 * A391681  Number of integer compositions of n with all parts > 1 that are
 *          not the first sums of any composition.
 * https://oeis.org/A391681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) -2*a(n-3) -6*a(n-4) - a(n-5) +4*a(n-6)
 *            +2*a(n-7) Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 *            a(5)=0, a(6)=0, a(7)=1 (And a(n)=0 for n<7). This recurrence
 *            follows from the generating function G(x) = x^7/(1 - x
 *            -4x^2+2x^3+6x^4+x^5-4x^6-2x^7)
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* capacity of cache (size in terms of n+1) */
    static idx_t last_computed = 0; /* highest n for which cache[n] is valid */

    if (n >= cache_cap) {
        /* grow cache to hold up to n */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – return 0 as safe default */
        /* initialise new entries to 0 */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values that are not yet computed */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 7) {
            cache[i] = 0;
        } else if (i == 7) {
            cache[i] = 1;
        } else {
            /* use the recurrence; all operations are safe for unsigned seq_t */
            cache[i] = cache[i - 1];
            cache[i] += cache[i - 2] * ((seq_t)4);
            cache[i] += cache[i - 6] * ((seq_t)4);
            cache[i] += cache[i - 7] * ((seq_t)2);
            cache[i] -= cache[i - 3] * ((seq_t)2);
            cache[i] -= cache[i - 4] * ((seq_t)6);
            cache[i] -= cache[i - 5];
        }
    }
    last_computed = n;
    return cache[n];
}
