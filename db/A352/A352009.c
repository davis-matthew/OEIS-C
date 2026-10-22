/*
 * A352009  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} (-2)^k * a(k) *
 *          a(n-2*k-1).
 * https://oeis.org/A352009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 33, i64 n <= 64, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = Σ_{k=0..⌊(n−1)/2⌋} (-2)^k * a(k) * a(n−2k−1)
   (OEIS A352009) */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* cached sequence values, grows as needed */
    static seq_t *cache = NULL;
    /* capacity of the cached array */
    static idx_t cap   = 0;
    /* index of the largest value already computed */
    static idx_t last  = 0;   /* last computed index; 0 after initialisation */

    /* ensure array large enough for indices up to n */
    if (cache == NULL) {
        /* first allocation: size n+1 */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cap   = n + 1;
    } else if (n + 1 > cap) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cap   = n + 1;
    }

    /* initialise a(0) if this is the first run */
    if (cap >= 1 && cache[0] == 0 && last == 0 && cache == NULL) {
        cache[0] = 1;
        last = 0;
    }
    if (last == 0 && cache[0] == 0) {   /* handle case where cache was just allocated */
        cache[0] = 1;
        last = 0;
    }

    /* compute values up to n if necessary */
    for (idx_t m = last + 1; m <= n; ++m) {
        if (m == 0) {            /* base case, already set */
            cache[m] = 1;
            continue;
        }

        seq_t sum  = 0;
        seq_t factor = 1;        /* (-2)^k, starts with k = 0 */
        idx_t maxk = (m - 1) / 2;/* floor((m-1)/2) */

        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t idx = m - 2 * k - 1;  /* n - 2k - 1, guaranteed valid */
            /* accumulate term: (-2)^k * a(k) * a(idx) */
            sum += factor * cache[k] * cache[idx];
            factor *= (seq_t)(-2);     /* prepare (-2)^(k+1) for next iteration */
        }
        cache[m] = sum;
    }

    last = n;   /* update last computed index */
    return cache[n];
}
