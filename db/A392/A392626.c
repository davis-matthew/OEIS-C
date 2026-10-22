/*
 * A392626  a(n) is the smallest k such that omega(k*n) = omega(k + n) where
 *          omega = A001221.
 * https://oeis.org/A392626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that ω(k*n) = ω(k + n) * where ω is
 *            the number of distinct prime factors (OEIS A001221)
 * Range:     u8 n <= 2309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/count_distinct_primes.h"

/* --- Sieve for counting distinct prime factors up to LIMIT --- */
#define SIEVE_LIMIT 4000000ULL

static uint32_t *spf = NULL;            /* smallest prime factor */
static uint8_t *omega_arr = NULL;       /* ω(i) for i <= SIEVE_LIMIT */
static int sieve_initialized = 0;       /* flag */

static void init_sieve(void)
{
    if (sieve_initialized)
        return;

    spf = (uint32_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    omega_arr = (uint8_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint8_t));
    if (!spf || !omega_arr)
        return; /* allocation failed – unlikely in tests */

    spf[0] = 0;
    spf[1] = 1;
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i)
        spf[i] = (uint32_t)i;

    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {            /* i is prime */
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
            }
        }
    }

    omega_arr[0] = 0;
    omega_arr[1] = 0;
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i) {
        uint32_t p = spf[i];
        uint64_t temp = i / p;
        while (temp % p == 0)
            temp /= p;
        omega_arr[i] = 1 + omega_arr[(size_t)temp];
    }

    sieve_initialized = 1;
}

/* return distinct prime factor count for x (Uint64) */
static int distinct_omega(uint64_t x)
{
    if (x <= SIEVE_LIMIT)
        return omega_arr[(size_t)x];
    return count_distinct_primes(x);
}

/* --- Cache for sequence values --- */
static seq_t *a_cache = NULL;   /* cache[0] unused; cache[n] = a(n) */
static idx_t a_cache_len = 0;   /* allocated size */

static void ensure_cache(idx_t n)
{
    if (n < a_cache_len)
        return;
    size_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failed – unlikely in tests */
    /* initialize new slots to 0 */
    for (size_t i = a_cache_len; i < new_len; ++i)
        new_cache[i] = 0;
    a_cache = new_cache;
    a_cache_len = new_len;
}

/* --- Main sequence function --- */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0, return 0 */
        return (seq_t)0;

    init_sieve();
    ensure_cache(n);

    if (a_cache[n] != (seq_t)0)   /* already computed */
        return a_cache[n];

    for (idx_t k = 1; ; ++k) {
        /* compute ω(k*n) and ω(k + n) */
        uint64_t prod = (uint64_t)n * (uint64_t)k;
        int o_prod = distinct_omega(prod);
        int o_sum  = distinct_omega((uint64_t)(k + n));
        if (o_prod == o_sum) {
            seq_t ans = (seq_t)k;
            a_cache[n] = ans;
            return ans;
        }
    }
}
