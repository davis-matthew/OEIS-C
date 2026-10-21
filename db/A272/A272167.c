/*
 * A272167  a(n) = Product_{k=2..n} (k^2-k)^k.
 * https://oeis.org/A272167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=2..n} (k^2 - k)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* --------- static cache of computed terms -------------------- */
    static seq_t *cache      = NULL;   /* cache[i] holds a(i) (i>=1) */
    static idx_t  cache_len  = 0;      /* allocated size of cache   */

    /* --------- ensure cache can hold index n -------------------- */
    if (cache_len <= n) {
        idx_t new_len = n + 1;                         /* indices 0..n */
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;                /* allocation failure */
        /* copy existing data */
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        /* initialise new slots to zero */
        for (idx_t i = cache_len; i < new_len; ++i) new_cache[i] = (seq_t)0;
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    /* --------- base case: a(1) = 1 ----------------------------- */
    if (n == 1) {
        if (cache_len <= 1) cache[1] = (seq_t)1;
        else if (cache[1] == (seq_t)0) cache[1] = (seq_t)1;
        return cache[1];
    }

    /* --------- initialise a(1) if not yet done ------------------- */
    if (cache[1] == (seq_t)0) cache[1] = (seq_t)1;

    /* --------- compute missing terms up to n --------------------- */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;           /* already computed */
        seq_t prev   = cache[i-1];
        seq_t base   = (seq_t)i * (seq_t)(i-1);        /* k*(k-1) */
        seq_t term   = big_pow(base, (seq_t)i);         /* (k^2-k)^k */
        cache[i] = seq_mul(prev, term);                /* a(i) = a(i-1)*term */
    }

    return cache[n];
}
