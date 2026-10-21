/*
 * A286363  Least number with the same prime signature as {the largest divisor
 *          of n with only prime factors of the form 4k+3} has: a(n) =
 *          A046523(A097706(n)).
 * https://oeis.org/A286363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A097706(n)) A097706(n) is the largest divisor of
 *            n whose prime factors are all ≡3 (mod 4). A046523 returns the
 *            least integer having the same multiset of exponents as its
 *            argument. The implementation factors n, keeps only the primes ≡3
 *            (mod 4), collects their exponents, sorts them i...
 * Range:     u8 n <= 6560, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*  ----  helper from the utils library  ----  */
#include "../utils/_smallest_prime_factor.h"   /* gives uint64_t _smallest_prime_factor(uint64_t n) */

/*  ----  cache of the first primes (ascending)  ----  */
static uint64_t *prime_cache      = NULL;
static size_t   prime_cache_size  = 0;   /* allocated capacity   */
static size_t   prime_cache_count = 0;   /* number of primes stored */
static bool     prime_cache_ready = false;

/*  simple primality test using the cached primes  */
static bool
is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cache_count; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/*  generate primes until we have at least need primes  */
static void
ensure_prime_cache(size_t need)
{
    if (prime_cache_ready) {
        if (prime_cache_count >= need) return;
    }
    /* initial capacity */
    size_t cap = prime_cache_size > 0 ? prime_cache_size : 64;
    if (cap < need) cap = need;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_cache = tmp;
    prime_cache_size = cap;
    /* if cache is empty, start from 2 */
    uint64_t cur = prime_cache_count == 0 ? 2 : prime_cache[prime_cache_count - 1] + 1;
    while (prime_cache_count < need) {
        if (is_prime_uint64(cur)) {
            prime_cache[prime_cache_count++] = cur;
        }
        ++cur;
    }
    prime_cache_ready = true;
}

/*  minimal number having the same multiset of exponents as the
    4k+3 prime part of n  */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;   /* index below 1 is unused */

    /*  collect exponents of primes ≡3 (mod 4)  */
    uint8_t exps[128] = {0};
    size_t  epc       = 0;                 /* number of collected exponents */

    uint64_t m = (uint64_t)n;
    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        if (p == 0) {                      /* safety, shouldn't happen */
            m = 1;
            break;
        }
        size_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if ((p & 3) == 3) {                /* p ≡ 3 (mod 4) */
            if (epc >= sizeof(exps)) {
                abort();                   /* maybe overflow, but unlikely */
            }
            exps[epc++] = (uint8_t)e;
        }
    }

    if (epc == 0) return (seq_t)1;          /* no 3(mod4) factors → result 1 */

    /*  sort exponents in non‑increasing order (simple insertion sort)  */
    for (size_t i = 1; i < epc; ++i) {
        size_t j = i;
        while (j > 0 && exps[j-1] < exps[j]) {
            uint8_t tmp = exps[j];
            exps[j] = exps[j-1];
            exps[j-1] = tmp;
            --j;
        }
    }

    /*  insure we have at least epc primes  */
    ensure_prime_cache(epc);

    /*  build the least number with this signature  */
    seq_t result = 1;
    for (size_t k = 0; k < epc; ++k) {
        seq_t base = (seq_t)prime_cache[k];
        seq_t e    = (seq_t)exps[k];
        while (e--) {
            result *= base;
        }
    }
    return result;
}
