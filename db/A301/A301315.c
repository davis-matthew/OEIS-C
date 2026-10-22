/*
 * A301315  Multiplicative with a(p^e) = prime(p) ^ a(e) (where prime(k)
 *          denotes the k-th prime number).
 * https://oeis.org/A301315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 161,
 *            u128 n <= 255, big any n
 * Helpers:   ensure_cache, factor_u64, get_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: a(p^e) = prime(p)^a(e),  a(1) = 1 */

/* Helper functions from the utilities library. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Static cache for memoisation.  cache[0] is unused; 1‑based indexing. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Base case.  Cache the value for 1 to avoid repeated recursion. */
    if (n == 1) {
        ensure_cache(1, &cache, &cache_cap);
        cache[1] = (seq_t)1;
        return (seq_t)1;
    }

    /* Make sure the cache is large enough for index n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Return the value if it has been computed earlier. */
    if (cache[n] != 0)
        return cache[n];

    /* Factorise n: acquire prime factors and exponents. */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute the multiplicative value from the factorisation. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        /* Fetch the (p‑th) prime: p is a prime number itself. */
        seq_t base = (seq_t)get_prime((idx_t)pr[i]);

        /* Recursively compute a(e). */
        seq_t exp = a((idx_t)ex[i]);

        /* Multiply by base^exp. */
        seq_t power = seq_pow(base, (idx_t)exp);
        result = seq_mul(result, power);
    }

    /* Store the result and return it. */
    cache[n] = result;
    return result;
}
