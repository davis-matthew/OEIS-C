/*
 * A304032  Number of ways to write 2*n as p + 2^k + 3^m with p prime and 2^k
 *          + 3^m a product of at most two distinct primes, where k and m are
 *          nonnegative integers.
 * https://oeis.org/A304032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A304032: Number of representations of 2*n as p + 2^k + 3^m
   with p prime and 2^k + 3^m a prime or a product of two distinct
   primes.  Computation is performed by enumerating all feasible
   powers of 2 and 3, checking the conditions with a cached list of
   primes. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"

static uint64_t u64_isqrt(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            if (mid == hi) break;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static idx_t    current_limit = 0;

static void ensure_prime_list(idx_t n)
{
    idx_t target  = 2 * n;
    idx_t limit   = u64_isqrt((uint64_t)target) + 1;
    if (limit <= current_limit) return;
    if (prime_list) free(prime_list);
    sieve_primes(limit, &prime_list, &prime_count);
    current_limit = limit;
}

static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Return true if s is prime or a product of *exactly* two distinct primes. */
static bool allowed_sum(uint64_t s)
{
    if (is_prime_uint64(s)) return true;

    int distinct = 0;
    uint64_t temp = s;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
                if (exp > 1) return false;   /* repeated prime factor */
            }
            distinct++;
            if (distinct > 2) return false;
        }
    }
    if (temp > 1) {
        /* remaining factor is prime */
        distinct++;
        if (distinct > 2) return false;
    }
    return distinct == 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined offset 0, safe fallback */

    ensure_prime_list(n);

    uint64_t target = 2 * (uint64_t)n;

    /* Precompute powers of 2 up to target. */
    uint64_t pow2[64];
    size_t p2cnt = 0;
    uint64_t val = 1;
    while (val <= target) {
        pow2[p2cnt++] = val;
        if (val > target / 2) break;
        val <<= 1;   /* val *= 2 */
    }

    /* Precompute powers of 3 up to target. */
    uint64_t pow3[64];
    size_t p3cnt = 0;
    val = 1;
    while (val <= target) {
        pow3[p3cnt++] = val;
        if (val > target / 3) break;
        val *= 3;
    }

    seq_t result = 0;

    for (size_t ki = 0; ki < p2cnt; ++ki) {
        uint64_t v2 = pow2[ki];
        if (v2 > target) break;
        for (size_t mi = 0; mi < p3cnt; ++mi) {
            uint64_t v3 = pow3[mi];
            uint64_t s = v2 + v3;
            if (s > target) break;
            uint64_t p_val = target - s;
            if (p_val < 2) continue;                /* p must be prime >=2 */
            if (!is_prime_uint64(p_val)) continue;
            if (!allowed_sum(s)) continue;
            ++result;
        }
    }

    return result;
}
