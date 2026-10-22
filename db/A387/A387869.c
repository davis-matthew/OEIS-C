/*
 * A387869  a(n) = Sum_{k=0..n} binomial(2*n+1,4*k).
 * https://oeis.org/A387869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+1,4*k) Recurrence: a(0)=1,
 *            a(1)=1, a(2)=6, a(n) = 4*a(n-1) - 4*a(n-2) + 16*a(n-3) for n>2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* static cache of values */
static idx_t cache_len = 0;   /* number of values stored */

seq_t a(idx_t n)
{
    /* ensure cache has at least n+1 entries */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure, return 0 */
        cache[0] = (seq_t)1;      /* a(0) */
    }

    if (n < cache_len)
        return cache[n];

    /* grow cache to size n+1 */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!tmp) return 0; /* allocation failure */
    cache = tmp;
    cache_len = n + 1;

    /* initialise missing values if necessary */
    if (n >= 1 && cache_len > 1) {
        cache[1] = (seq_t)1; /* a(1) */
    }
    if (n >= 2 && cache_len > 2) {
        cache[2] = (seq_t)6; /* a(2) */
    }

    /* compute missing values using recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t t1 = (seq_t)4 * cache[i - 1];
        seq_t t2 = (seq_t)4 * cache[i - 2];
        seq_t sub = t1 - t2;                  /* 4*a[i-1] - 4*a[i-2] */
        seq_t t3 = (seq_t)16 * cache[i - 3];
        cache[i] = sub + t3;                  /* recurrence */
    }

    return cache[n];
}
