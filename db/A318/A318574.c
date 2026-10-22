/*
 * A318574  Denominator of the reciprocal sum of the integer partition with
 *          Heinz number n.
 * https://oeis.org/A318574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the sum of the exponents divided by the
 *            index of the prime in n's prime factorisation. If n = ∏
 *            prime_i^{e_i} and index(p_i) = i, then a(n) = denom( Σ e_i / i )
 *            in lowest terms
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   build_primes_upto, ensure_cache, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ensure_cache.h"
#include "../utils/build_primes_upto.h"

/* Memoization cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Array that maps a prime value to its 1‑based index:
   prime_index_arr[p] = index of prime p, 0 if p not prime.
   The array is valid up to prime_idx_cap. */
static uint32_t *prime_index_arr = NULL;
static idx_t prime_idx_cap = 0;

/* Ensure prime_index_arr contains indices for all primes ≤ limit */
static void ensure_prime_index(idx_t limit)
{
    if (prime_idx_cap > limit) return;

    /* Generate all primes up to limit.  The array returned by
       build_primes_upto holds the primes themselves.                 */
    uint64_t *pr = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &pr, &cnt);

    /* Allocate an array large enough to map each integer up to limit. */
    uint32_t *new_arr = (uint32_t*) calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!new_arr) return;   /* leave global state unchanged on OOM */

    for (size_t i = 0; i < cnt; ++i)
        if (pr[i] <= (uint64_t)limit)
            new_arr[(size_t)pr[i]] = (uint32_t)(i + 1);  /* 1‑based index */

    free(pr);

    free(prime_index_arr);
    prime_index_arr = new_arr;
    prime_idx_cap = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for 0 but just in case */

    /* Expand cache if necessary and initialise new slots to 0 */
    if (n >= cache_cap) {
        size_t old_cap = cache_cap;
        ensure_cache(n + 1, &cache, &cache_cap);
        for (size_t i = old_cap; i < cache_cap; ++i)
            cache[i] = 0;
    }

    /* Return memoised value if available */
    if (cache[n] != 0) return cache[n];

    /* Make sure we have prime indices up to n (n is a prime itself at
       most). */
    ensure_prime_index(n);

    /* Factorise n */
    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t cnt_f = 0;
    factor_u64((uint64_t)n, primes_f, exps_f, &cnt_f);

    /* Sum the fractions e / index(p) */
    seq_t num = 0;   /* numerator of the running sum */
    seq_t den = 1;   /* denominator of the running sum */

    for (size_t i = 0; i < cnt_f; ++i) {
        uint64_t p  = primes_f[i];
        seq_t e     = (seq_t)exps_f[i];
        seq_t idx   = (seq_t)prime_index_arr[p];   /* 1‑based index */

        /* num/den + e/idx = (num*idx + e*den) / (den*idx) */
        seq_t new_num = num * idx + e * den;
        seq_t new_den = den * idx;

        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    cache[n] = den;
    return den;
}
