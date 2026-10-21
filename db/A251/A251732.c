/*
 * A251732  a(n) = 3^n*A123335(n). Rational parts of the integers in
 *          Q(sqrt(2)) giving the length of a Lévy C-curve variant at
 *          iteration step n.
 * https://oeis.org/A251732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -6*a(n-1) + 9*a(n-2), a(0)=1, a(1)=-3
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of stored terms (indices 0 … cache_len-1) */

    /* initialise cache with the first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* unlikely, but keep the function well‑defined */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-3;
    }

    /* return already computed value if possible */
    if (n < cache_len) return cache[n];

    /* extend cache to accommodate index n */
    idx_t old_len = cache_len;
    cache_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;            /* allocation failure handled gracefully */
    cache = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = old_len; i <= n; ++i) {
        cache[i] = -6 * cache[i - 1] + 9 * cache[i - 2];
    }

    return cache[n];
}
