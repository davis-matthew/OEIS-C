/*
 * A395619  For any n > 0 with prime factorization Prod_{i = 1..k} p_i^e_i
 *          where p_1 < ... < p_k and e_1, ..., e_k > 0, a(n) = Prod_{i =
 *          1..k} prime(i)^a(e_i) (where prime(i) denotes the i-th prime
 *          numbers).
 * https://oeis.org/A395619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..k} prime(i)^{a(e_i)} where n = ∏_{i=1..k}
 *            p_i^{e_i} and p_1 < p_2 < … < p_k
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Memoisation cache */
static seq_t  *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold values up to n */
static void
ensure_cache_size(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = n * 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof *cache);
    if (!tmp) abort();
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used, safety */
    if (n == 1) return 1;            /* a(1) = 1 */

    ensure_cache_size(n);
    if (cache[n - 1] != 0) return cache[n - 1];

    /* factorise n */
    uint64_t primes_arr[16];
    uint64_t exps_arr[16];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes_arr, exps_arr, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps_arr[i];
        /* prime index is i+1 (because p_1 < p_2 < ... < p_k) */
        int  prime_val = get_prime((idx_t)(i + 1));   /* global i‑th prime */
        seq_t exp_val  = a((idx_t)e);                 /* recursion on exponent */
        seq_t term     = big_pow((seq_t)prime_val, exp_val);
        prod *= term;
    }

    cache[n - 1] = prod;
    return prod;
}
