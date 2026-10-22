/*
 * A375031  Numbers whose prime factorization has at least one exponent that
 *          equals 2 and no higher even exponent.
 * https://oeis.org/A375031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A375031: Numbers whose prime factorization contains at least one exponent equal to 2 and no higher even exponent.
   The implementation uses a simple sieve of primes and a cache of known terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for uint64_t. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 0xFFFFFFFFULL;     /* high^2 > 2^64-1 */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t m2 = (__uint128_t)mid * (__uint128_t)mid;
        if (m2 == n) return mid;
        if (m2 < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Cache of discovered terms (1‑based). */
static seq_t *cache   = NULL;
static size_t  cache_sz  = 0;
static size_t  cache_cap = 0;

/* Array of primes for trial division. */
static uint64_t *primes   = NULL;
static size_t    primes_sz  = 0;
static size_t    primes_cap = 0;
static uint64_t  primes_max = 1;           /* primes are built up to this value */

/* Ensure the prime array can hold at least 'need' entries. */
static void ensure_primes_cap(size_t need)
{
    if (primes_cap >= need) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = new_cap;
}

/* Extend the prime list up to 'limit' (inclusive) by trial division. */
static void sieve_primes_up_to(uint64_t limit)
{
    if (limit <= primes_max) return;
    for (uint64_t cand = primes_max + 1; cand <= limit; ++cand) {
        bool isprime = true;
        uint64_t r = isqrt_u64(cand);
        for (size_t i = 0; i < primes_sz && primes[i] <= r; ++i) {
            if (cand % primes[i] == 0) { isprime = false; break; }
        }
        if (isprime) {
            ensure_primes_cap(primes_sz + 1);
            primes[primes_sz++] = cand;
        }
    }
    primes_max = limit;
}

/* Test whether a uint64_t satisfies the exponent condition. */
static bool good_uint64(uint64_t n)
{
    if (n < 4) return false;           /* smallest term is 4 */
    bool has_two = false;
    uint64_t rem = n;

    uint64_t lim = isqrt_u64(rem);
    sieve_primes_up_to(lim);

    for (size_t i = 0; i < primes_sz && primes[i] * primes[i] <= rem; ++i) {
        if (rem % primes[i] == 0) {
            int e = 0;
            while (rem % primes[i] == 0) {
                rem /= primes[i];
                ++e;
            }
            if (e == 2) has_two = true;
            else if ((e & 1) == 0) /* even exponent > 2 */
                return false;
        }
    }
    /* Remaining factor is prime with exponent 1 (odd). */
    return has_two;
}

/* Ensure that the cache contains at least 'n' terms (1‑based). */
static void ensure_cache_len(idx_t n)
{
    if ((size_t)n <= cache_sz) return;
    static uint64_t last_tested = 1;   /* last integer examined (as uint64_t) */
    while (cache_sz < (size_t)n) {
        ++last_tested;
        uint64_t lim = isqrt_u64(last_tested);
        sieve_primes_up_to(lim);
        if (good_uint64(last_tested)) {
            if (cache_cap <= cache_sz) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)last_tested;
        }
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache_len(n);
    return cache[n - 1];
}
