/*
 * A387857  Number of nonnegative s < n such that s^s == -(-s)^n (mod n).
 * https://oeis.org/A387857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of s ∈ [0, n) such that s^s ≡ -(-s)^n (mod n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_mod64.h"   /* uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod) */

static inline uint64_t mod_pow_u64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0)
    {
        if (exp & 1U)
            result = mul_mod64(b, result, mod);
        b = mul_mod64(b, b, mod);
        exp >>= 1U;
    }
    return result;
}

/*-- Cache for computed values ------------------------------------*/
static seq_t      *cache   = NULL;   /* array index 0..n */
static size_t     cache_cap   = 0;   /* allocated capacity */
static size_t     computed_n  = 0;   /* highest n computed */

/* Grow the cache to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_cap2 = cache_cap ? cache_cap * 2 : 16;
    if (new_cap2 < new_cap)
        new_cap2 = new_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap2 * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    cache = tmp;
    cache_cap = new_cap2;
}

seq_t a(idx_t n)
{
    /* If already cached, return immediately */
    if (n <= computed_n)
        return cache[n];

    /* Ensure cache large enough */
    grow_cache((size_t)n + 1);

    /* Compute values from (computed_n+1) up to n */
    for (size_t k = computed_n + 1; k <= (size_t)n; ++k)
    {
        seq_t count = 0;
        uint64_t mod = (uint64_t)k;     /* n fits in uint64_t */

        for (idx_t s = 0; s < k; ++s)
        {
            uint64_t base_pow  = mod_pow_u64((uint64_t)s, s, mod);
            uint64_t base_neg  = (mod - (uint64_t)s) % mod;
            uint64_t pow_neg   = mod_pow_u64(base_neg, k, mod);
            uint64_t rhs       = (mod - pow_neg) % mod;   /* -(-s)^n mod n */

            if (base_pow == rhs)
                count += (seq_t)1;
        }
        cache[k] = count;
    }

    computed_n = (size_t)n;
    return cache[n];
}
