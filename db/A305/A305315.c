/*
 * A305315  a(n) = sqrt(5*b(n)^2 - 4), with b(n) = A134493(n) =
 *          Fibonacci(6*n+1), n >= 0.
 * https://oeis.org/A305315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) – a(n-2), with a(0)=1, a(1)=29
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=29, a(n)=18*a(n-1)-a(n-2) for n≥2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoization cache */
    static idx_t cache_len = 0;     /* number of terms stored */
    static idx_t cache_cap = 0;     /* allocated capacity */

    /* base case n==0 */
    if (n == 0) {
        if (cache_len == 0) {
            cache_cap = 2;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            cache[0] = 1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* initialise cache if empty */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache_len = 1;
    }

    /* extend cache capacity if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 18 * cache[0] + 11;        /* since a(1)=18*a(0)-a(-1)=18*1+11=29 */
        } else {
            val = 18 * cache[i - 1] - cache[i - 2];
        }
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
