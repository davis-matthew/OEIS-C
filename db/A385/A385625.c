/*
 * A385625  Sum of the divisors d of n with an odd number of primes not
 *          exceeding d.
 * https://oeis.org/A385625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * (pi(d) mod 2)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* storage for precomputed primes */
static uint64_t *primes = NULL;       /* array of primes */
static size_t   primes_cnt = 0;       /* number of primes stored */
static idx_t    primes_limit = 0;     /* maximum n up to which primes are known */

/* Ensure that all primes <= limit are generated. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= primes_limit)
        return;
    uint64_t *new_primes = NULL;
    size_t   new_cnt   = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);
    free(primes);
    primes = new_primes;
    primes_cnt = new_cnt;
    primes_limit = limit;
}

/* Recursive enumeration of all divisors from factor list. */
static void enumerate_divisors(const uint64_t *factors,
                               const uint64_t *exps,
                               size_t fac_cnt,
                               size_t pos,
                               uint64_t cur,
                               seq_t *sum,
                               const uint64_t *prime_arr,
                               size_t prime_cnt)
{
    if (pos == fac_cnt) {
        /* Count primes <= cur using binary search */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_arr[mid] <= cur)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo & 1)          /* odd parity of pi(cur) */
            *sum += (seq_t)cur;
        return;
    }
    uint64_t p = factors[pos];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        enumerate_divisors(factors, exps, fac_cnt, pos + 1, val, sum,
                           prime_arr, prime_cnt);
        if (e < exps[pos])
            val = val * p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety, caller supplies n>=1 */

    /* Need primes up to n for counting pi(d). */
    ensure_primes_up_to(n);

    /* Factor n using precomputed primes. */
    uint64_t tmp = (uint64_t)n;
    uint64_t factors[64];
    uint64_t exps[64];
    size_t fac_cnt = 0;

    for (size_t i = 0; i < primes_cnt &&
         (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            factors[fac_cnt] = p;
            exps[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (tmp > 1) {        /* remaining factor is prime */
        factors[fac_cnt] = tmp;
        exps[fac_cnt] = 1;
        ++fac_cnt;
    }

    seq_t sum = 0;
    enumerate_divisors(factors, exps, fac_cnt, 0, 1, &sum,
                       primes, primes_cnt);
    return sum;
}
