/*
 * A380922  Dirichlet g.f.: zeta(s) * Product_{p prime} (1 + 1/p^s +
 *          1/p^(3*s)).
 * https://oeis.org/A380922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The sequence is multiplicative with
 *   a(p^e) = 2  if e <= 2  (prime powers p^1 or p^2)
 *          = 3  if e >= 3
 * So a(n) = Π_{p^e||n} (e<=2 ? 2 : 3)
 * This function uses factor_u64 for factorisation and memoises values
 * in a static cache so that calls in any order are fast. */

/* External helper for factorisation.
 * description: factor_u64(uint64_t n,
 *                         uint64_t *primes,
 *                         uint64_t *exponents,
 *                         size_t *cnt)
 * returns the distinct primes and their exponents of n. */
#include "../utils/factor_u64.h"

/* External integer types supplied by the build. */
extern seq_t a(idx_t n);

/* Cache: cache[1] holds a(1). */
static seq_t *cache = NULL;
static idx_t max_cache = 0;   /* highest n computed so far */

/* Compute a(n) from multiplicative definition and store in cache[n]. */
static void compute_value(idx_t n)
{
    seq_t val = (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] <= 2ULL)
            val *= (seq_t)2;
        else
            val *= (seq_t)3;
    }
    cache[n] = val;
}

/* Ensure the cache has stored values up through n. */
static void ensure_cache(idx_t n)
{
    if (n <= max_cache)
        return;

    size_t new_cap = (size_t)n + 1;          /* allocate n+1 elements */
    if (new_cap < 1024)
        new_cap = 1024;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                 /* fatal allocation error */

    /* initialise any newly allocated entries to zero */
    for (size_t i = max_cache + 1; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;
    cache = new_cache;

    /* compute values for indices that are now available */
    for (idx_t i = max_cache + 1; i <= n; ++i)
        compute_value(i);

    max_cache = n;
}

/* Public entry point required by the build harness. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, return 0 safely */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
