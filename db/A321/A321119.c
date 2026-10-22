/*
 * A321119  a(n) = ((1 - sqrt(3))^n + (1 + sqrt(3))^n)/2^floor((n - 1)/2);
 *          n-th row common denominator of A321118.
 * https://oeis.org/A321119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 4*a(n-2) - a(n-4) Initial values: a(0)=4, a(1)=2, a(2)=8,
 *            a(3)=10
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static idx_t last_computed = 3;   /* highest index computed so far (>=3) */
    static idx_t cache_cap   = 0;      /* allocated capacity */

    /* initialise cache with the first four terms if necessary */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)10;
    }

    /* handle small indices directly */
    if (n <= 3)
        return cache[(idx_t)n];

    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n using the recurrence a(n)=4*a(n-2)-a(n-4) */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* 4*cache[i-2] and cache[i-4] are performed in the natural
           type of seq_t; the results wrap modulo 2^bits for fixed‑size
           types and are exact for arbitrary‑precision types. */
        cache[i] = 4 * cache[i - 2] - cache[i - 4];
    }
    last_computed = n;
    return cache[n];
}
