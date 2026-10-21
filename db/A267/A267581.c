/*
 * A267581  Decimal representation of the middle column of the "Rule 167"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 1 - [ (2^(n+1)) mod n == 0 ], a(0) = 1, a(1) =
 *            3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2mod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A267581 using the proven
   recurrence.  The function is memoised so that each new term is
   computed only once, regardless of the order of calls. */
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2mod_uint64.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* initialise cache with first two terms */
    if (cache_len == 0) {
        idx_t init_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * init_cap);
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
        cache_cap = init_cap;
    }

    if (n < cache_len)
        return cache[n];

    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) exit(1);
        memcpy(new_cache, cache, sizeof(seq_t) * cache_len);
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t val  = prev << 1;  /* 2*prev */
        val += (seq_t)1;

        /* subtract 1 iff (2^(i+1)) mod i == 0  */
        uint64_t mod  = (uint64_t)i;
        uint64_t exp  = (uint64_t)(i + 1);
        uint64_t pow  = pow2mod_uint64(exp, mod);
        if (pow == 0)
            val -= (seq_t)1;

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
