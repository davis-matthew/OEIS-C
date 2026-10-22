/*
 * A375823  Number of ways to tile a 3-row trapezoid of average length n with
 *          triangular and rectangular tiles, each of size 3.
 * https://oeis.org/A375823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-3) - a(n-4) - a(n-6) initial values:
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=6, a(4)=16, a(5)=43
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS A375823.
   Uses memoised recurrence and static cache for speed. */
seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache = NULL;   /* array of computed values */
    static size_t cap    = 0;     /* allocated capacity (in elements) */
    static idx_t max_computed = (idx_t)-1; /* index of last computed term */

    /* initialise cache on first call */
    if (max_computed == (idx_t)-1) {
        /* need at least 6 slots for the base cases a(0)..a(5) */
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failure – return 0 */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)16;
        cache[5] = (seq_t)43;
        max_computed = 5;
    }

    /* if requested value already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* grow the array if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {            /* realloc failed – return 0 */
            free(cache);
            cache = NULL;
            cap = 0;
            max_computed = (idx_t)-1;
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute new terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val = val * (seq_t)2;
        val += cache[i - 3] * (seq_t)4;
        val -= cache[i - 4];
        val -= cache[i - 6];
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
