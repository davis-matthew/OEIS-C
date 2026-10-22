/*
 * A369816  Expansion of 1/(1 - x^5 - x^7).
 * https://oeis.org/A369816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + a(n-7), a(0)=1, a(n)=0 for n<0
 * Range:     u8 n <= 62, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of compositions of n into parts 5 and 7 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache      = NULL;  /* pointer to the array */
    static idx_t capacity    = 0;     /* allocated capacity */
    static idx_t max_computed = (idx_t)-1;  /* largest index computed */

    /* enlarge capacity if necessary */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity ? capacity * 2 : 1;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute values up to n if they are missing */
    if (max_computed == (idx_t)-1) {            /* nothing computed yet */
        cache[0] = (seq_t)1;                     /* a(0) = 1 */
        max_computed = 0;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 5) val += cache[i - 5];
        if (i >= 7) val += cache[i - 7];
        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
