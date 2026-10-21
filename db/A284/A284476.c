/*
 * A284476  Smallest number with same factorization shape as n.
 * https://oeis.org/A284476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..k} prime(i)^(f_i), where n = ∏ p_i^e_i and the
 *            f_i are the values a(e_i) sorted in non‑increasing order
 * Range:     u8 n <= 419, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t, ensure_cache, factor_u64, get_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"   /* factor_u64(n, primes, exps, &cnt) */
#include "../utils/get_prime.h"   /* get_prime(idx) */
#include "../utils/ensure_cache.h"/* ensure_cache(n, &cache, &cap) */
#include "../utils/pow_seq.h"     /* pow_seq(base, exp) */
#include "../utils/cmp_seq_t.h"   /* comparison routine for qsort */

/* Cache for already computed terms; index 0 is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Recursive helper that uses the cache. */
static seq_t a_rec(idx_t n)
{
    ensure_cache(n, &cache, &cache_cap);

    if (cache[n] != 0)            /* previously computed */
        return cache[n];

    if (n == 1) {                 /* base case */
        cache[n] = 1;
        return 1;
    }

    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps_arr[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps_arr, &cnt);

    /* Compute a(e) for each exponent e. */
    seq_t f_vals[64];
    for (size_t i = 0; i < cnt; ++i) {
        f_vals[i] = a_rec((idx_t)exps_arr[i]);
    }

    /* Sort the f_vals in ascending order. */
    qsort(f_vals, cnt, sizeof(seq_t), cmp_seq_t);

    /* Build the product with prime(i)^f_{k+1-i}. */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t prime_index = (idx_t)(i + 1);                      /* prime(i+1) */
        seq_t prime_val   = get_prime(prime_index);
        seq_t pow_val     = pow_seq(prime_val,
                                   (int)f_vals[cnt - 1 - i]);    /* descending f */
        result *= pow_val;
    }

    cache[n] = result;
    return result;
}

/* Public entry function. */
seq_t a(idx_t n)
{
    return a_rec(n);
}
