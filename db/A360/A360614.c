/*
 * A360614  Numerator of the average distance between consecutive 0-prepended
 *          prime indices of n; a(1) = 0.
 * https://oeis.org/A360614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 1618, u16 n <= 9466, u32 n <= 9466, u64 n <= 9466,
 *            u128 n <= 9466, big any n
 * Helpers:   build_primes_upto, gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = max_{p∣n} index(p) / gcd(max_{p∣n} index(p), Ω(n))
 * where Ω(n) is the total number of prime factors of n counted with multiplicity.
 * The set of "0‑prepended prime indices" of n consists of 0 followed by the
 * prime indices of each prime factor of n with multiplicity.
 * The average distance between consecutive elements in that sorted multiset
 * is max_index/Ω(n);  a(n) is the numerator of the reduced fraction.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"             /* seq_t gcd(seq_t, seq_t) */
#include "../utils/int_sqrt.h"        /* idx_t int_sqrt(idx_t) */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t, uint64_t **, size_t *) */

/* Cached prime array and its metadata */
static uint64_t *cached_primes   = NULL;
static size_t   cached_prime_cnt = 0;
static idx_t    cached_prime_lim = 0;

/* Ensure the cached array contains all primes up to 'limit' (inclusive). */
static void
ensure_primes_up_to(idx_t limit)
{
    if (cached_prime_lim >= limit)
        return;

    uint64_t *new_primes = NULL;
    size_t   new_cnt     = 0;

    build_primes_upto(limit, &new_primes, &new_cnt);

    free(cached_primes);
    cached_primes     = new_primes;
    cached_prime_cnt  = new_cnt;
    cached_prime_lim  = limit;
}

/* Binary search: return 1‑based index of prime 'p' in the cached primes array.
 * Assumes 'p' is prime and <= cached_prime_lim. */
static size_t
prime_index(uint64_t p)
{
    size_t lo = 0;
    size_t hi = cached_prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cached_primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo + 1;   /* 1‑based index */
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;   /* by definition */

    /* Prime factorisation of n using cached primes up to sqrt(n) */
    idx_t sqrt_n = int_sqrt(n);
    ensure_primes_up_to(sqrt_n);

    uint64_t temp   = (uint64_t)n;
    size_t   omega  = 0;                /* Ω(n) */
    uint64_t max_idx = 0;               /* maximum prime index */

    /* Trial division by all primes <= sqrt(n) */
    for (size_t i = 0; i < cached_prime_cnt && cached_primes[i] <= sqrt_n; ++i) {
        uint64_t p = cached_primes[i];
        if (temp % p != 0)
            continue;

        size_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        omega += exp;

        uint64_t idx = prime_index(p);
        if (idx > max_idx)
            max_idx = idx;
    }

    /* Remaining prime factor (if any) > sqrt(n) */
    if (temp > 1) {
        uint64_t remaining = temp;
        /* Ensure cached primes include this prime to get its index */
        if (remaining > cached_prime_lim)
            ensure_primes_up_to(remaining);

        omega += 1;
        uint64_t idx = prime_index(remaining);
        if (idx > max_idx)
            max_idx = idx;
    }

    /* Compute numerator = max_idx / gcd(max_idx, omega) */
    seq_t a_num = (seq_t)max_idx;
    seq_t a_den = (seq_t)omega;
    seq_t g     = gcd(a_num, a_den);
    return a_num / g;
}
