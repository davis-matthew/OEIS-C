/*
 * A390639  Numbers k such that the sum of k and twice the sum of its prime
 *          factors, counted with multiplicity, is prime.
 * https://oeis.org/A390639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 3845, u32 n <= 4132, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A390639:
 *  a(n) = the nth positive integer k such that k + 2 * A001414(k) is prime,
 *  where A001414(k) = sum of prime factors of k counted with multiplicity.  */

/*  Approach:
 *  • Generate a prime table once using a sieve up to a generous limit.
 *  • For each candidate k, sum its prime factors by trial division over the
 *    precomputed primes.
 *  • Test primality of k + 2 * sum using is_prime_big (handles arbitrary-precision).
 *  • Store found terms in a dynamic cache so that future calls can answer any
 *    n regardless of call order.
 *  Historically, memory allocations in C++ must be cast from void*, so all calls
 *  to malloc include a cast.  */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

static bool init_done = false;
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static const idx_t PRIME_LIMIT = 20000000;   /* 20 million should cover n <= 10000 */

static void init_primes(void)
{
    if (init_done) return;
    /* Build a list of all primes up to PRIME_LIMIT. */
    size_t cnt = 0;
    build_primes_upto(PRIME_LIMIT, &prime_list, &cnt);
    prime_count = cnt;
    init_done = true;
}

/* Sum of prime factors of n with multiplicity, using the precomputed prime list. */
static uint64_t sum_prime_factors_uint64(uint64_t n)
{
    uint64_t sum = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        while (temp % p == 0) {
            sum += p;
            temp /= p;
        }
    }
    if (temp > 1) sum += temp;   /* remaining prime factor */
    return sum;
}

/* --------- cache for computed terms --------------------------------- */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  last_candidate = 0;

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_arr, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* ---------- main sequence routine ----------------------------------- */
seq_t a(idx_t n)
{
    init_primes();                     /* ensure primes are ready */

    if (n == 0) return 0;              /* safety for offset 1 */

    if (n <= cache_len)                /* cached value */
        return cache[n - 1];

    grow_cache(n);

    for (idx_t cand = last_candidate + 1; ; ++cand) {
        uint64_t k = (uint64_t)cand;
        uint64_t sum_pf = sum_prime_factors_uint64(k);
        uint64_t test_val_U64 = k + 2 * sum_pf;
        seq_t test_val = (seq_t)test_val_U64;   /* convert to seq_t */
        if (is_prime_big(test_val)) {           /* prime test */
            cache[cache_len++] = (seq_t)k;
            if (cache_len == n) {
                last_candidate = cand;
                return cache[n - 1];
            }
        }
        last_candidate = cand;
    }
}
