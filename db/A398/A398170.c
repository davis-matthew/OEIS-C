/*
 * A398170  Exponentially odd numbers k such that k+1 is squarefree.
 * https://oeis.org/A398170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A398170: Exponentially odd integers k such that k+1 is squarefree.
   The function builds the sequence by scanning successive k starting from 1,
   keeping those for which every prime exponent in k is odd (checked by
   factor_u64) and for which k+1 has no repeated prime factor (squarefree).
   Results are cached so that a(idx_t n) can be called in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_realloc.h"

/* Cached sequence */
static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t  last_checked = 0; /* largest integer examined so far */

/* Expand the cache when needed; uses seq_realloc for arbitrary‑precision types. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return true iff all prime exponents in n are odd (n >= 1). */
static bool exp_odd_uint64(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if ((ex[i] & 1) == 0)   /* even exponent */
            return false;
    return true;  /* includes n=1 where cnt==0 */
}

/* Return true iff n is squarefree. */
static bool squarefree_uint64(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] > 1)   /* repeated prime factor */
            return false;
    return true;
}

/* Return the n‑th term of A398170. */
seq_t a(idx_t n)
{
    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        ++last_checked;               /* next candidate k */

        /* Test the two conditions. */
        if (exp_odd_uint64((uint64_t)last_checked) &&
            squarefree_uint64((uint64_t)(last_checked + 1)))
        {
            /* Store the term, enlarging the cache as necessary. */
            if (cache_len == cache_cap)
                grow_cache(cache_cap ? cache_cap * 2 : 128);
            cache[cache_len] = (seq_t)last_checked;   /* cast to seq_t */
            ++cache_len;
        }
    }
    return cache[n - 1];
}
