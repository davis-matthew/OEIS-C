/*
 * A394681  a(0) = 1 and a(n) = n^2*a(n-1) + n! for n > 0.
 * https://oeis.org/A394681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*a(n-1) + n! with a(0) = 1 (OEIS A394681)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[0..last] computed; fact_cur = last!   */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t  last    = 0;
    static seq_t  fact_cur = (seq_t)1;   /* factorial of last */

    /* first time: ensure cache[0]=1 and capacity at least n+1 */
    if (cache == NULL) {
        /* allocate enough for the requested n, at least 1 element */
        size_t init_cap = (n + 1 > 1) ? (size_t)(n + 1) : 1;
        /* round capacity up to a power of two for easier growth */
        while (init_cap & (init_cap - 1)) init_cap++;  /* not really needed for small n */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        cap = init_cap;
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        last = 0;
        fact_cur = (seq_t)1;           /* 0! = 1 */
    }

    /* expand capacity if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute new terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* factorial update: fact_cur = i! */
        fact_cur = fact_cur * (seq_t)i;

        /* a(i) = i^2 * a(i-1) + i! */
        seq_t i_seq = (seq_t)i;
        seq_t value = i_seq * i_seq * cache[i - 1] + fact_cur;
        cache[i] = value;
    }
    last = n;

    /* return the requested value (may already exist) */
    return cache[n];
}
