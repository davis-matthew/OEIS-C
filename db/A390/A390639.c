/*
 * A390639  Numbers k such that the sum of k and twice the sum of its prime
 *          factors, counted with multiplicity, is prime.
 * https://oeis.org/A390639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 3845, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9831 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
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
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
    seq_t *new_arr = mem_new(seq_t, new_cap);
    if (cache) {
        mem_copy(new_arr, cache, cache_len * sizeof(seq_t));
        mem_free(cache);
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
