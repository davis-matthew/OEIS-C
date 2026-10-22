/*
 * A322056  First differences of A322055.
 * https://oeis.org/A322056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322056 – first differences of A322055.
   Recurrence (derived from the generating function)
   a(n) = 2*a(n‑4) – a(n‑8) + c(n),   n ≥ 8
   where the correction term c(n) is
       c(8)  = 49,
       c(10) = –8,
       c(11) = 16,
       c(12) = –8,
       c(n)  = 0 otherwise.
   Initial values (a(0)…a(7)):
       1, 8, 32, 32, 72, 40, 136, 64.
   For n < 8 the values are returned directly.  The function is memoised
   so that calls in any order are efficient.  The recurrence holds for
   all n ≥ 13; for n = 8..12 the correction terms above are applied. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* memoisation array */
static size_t cache_size = 0;   /* number of elements stored */
static size_t cache_cap  = 0;   /* allocated capacity */

/* 8 initial terms, a(0) … a(7) */
static const seq_t init[8] = {
    (seq_t)1, (seq_t)8, (seq_t)32, (seq_t)32,
    (seq_t)72, (seq_t)40, (seq_t)136, (seq_t)64
};

/* returns the requested term */
seq_t a(idx_t n)
{
    /* handle initial segment directly */
    if (n < 8)
        return init[(size_t)n];

    /* ensure cache large enough */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        delete [] cache;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
#endif
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise cache with seeds if not yet done */
    if (cache_size < 8) {
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_size = 8;
    }

    /* compute values up to n using the recurrence */
    for (idx_t k = (idx_t)cache_size; k <= n; ++k) {
        /* 2*a(k-4) - a(k-8)  */
        seq_t val = 2 * cache[(size_t)(k - 4)] - cache[(size_t)(k - 8)];

        /* apply correction terms for k = 8..12 */
        if (k == 8)
            val += (seq_t)49;
        else if (k == 10)
            val -= (seq_t)8;
        else if (k == 11)
            val += (seq_t)16;
        else if (k == 12)
            val -= (seq_t)8;

        cache[(size_t)k] = val;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
