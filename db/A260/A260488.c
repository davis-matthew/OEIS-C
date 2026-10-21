/*
 * A260488  Numbers of the form 2^m * (6k + 1) for m, k >= 0, and 0.
 * https://oeis.org/A260488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates OEIS sequence A260488: numbers of the form 2^m*(6k+1) (m,k≥0) together with 0.
   The sequence is returned in increasing order, a(0)=0 as the first term.  */
#include <stdlib.h>
#include <stdbool.h>

/* Return the odd part of x (remove all factors of 2). */
static inline seq_t odd_part(seq_t x)
{
    while ((x & (seq_t)1) == (seq_t)0 && x != (seq_t)0)
        x >>= 1;
    return x;
}

/* a(n): n-th term (n ≥ 0).  The function is memoised independently of call order. */
seq_t a(idx_t n)
{
    /* Static cache of already generated terms. */
    static seq_t *cache  = NULL;   /* array holding the sequence values */
    static idx_t cache_len = 0;    /* number of elements stored   */
    static idx_t cache_cap = 0;    /* allocated capacity of cache */
    static seq_t cand       = (seq_t)0;   /* next candidate to test      */

    /* Grow cache until it contains at least n+1 terms. */
    while (cache_len <= n)
    {
        /* Is cand a member of the sequence? */
        if (cand == (seq_t)0 ||
            (odd_part(cand) % (seq_t)6) == (seq_t)1)
        {
            /* Ensure we have enough space. */
            if (cache_len == cache_cap)
            {
                cache_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache,
                                              cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = cand;
        }
        /* Move to the next integer. */
        cand++;
    }

    return cache[n];
}
