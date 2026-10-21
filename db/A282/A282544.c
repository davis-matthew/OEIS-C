/*
 * A282544  Expansion of (phi(x)^4 + 3*phi(x^3)^4) / 4 in powers of x where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A282544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * prod over prime powers of b(p^e) where b(2^e) = 3,
 *            b(3^e) = 3^(e+1) – 2, b(p^e) = (p^(e+1) – 1)/(p – 1) for p > 3,
 *            and a(0) = 1
 * Range:     u8 n <= 16, u16 n <= 1086, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   b_of_prime_power, ipow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/b_of_prime_power.h"
#include "../utils/ipow_seq.h"

/* simple integer square root for uint64_t   */
static inline uint64_t isqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = v, mid, res = 0;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (mid != 0 && mid > v / mid) { /* mid*mid > v, avoid overflow */
            hi = mid - 1;
        } else {
            res = mid;
            lo = mid + 1;
        }
    }
    return res;
}

/* dynamic prime sieve up to a given limit */
static uint64_t *prime_tbl = NULL;
static size_t    prime_cnt = 0;
static size_t    prime_cap = 0;
static uint64_t  prime_limit = 1;

static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;

    /* grow table if necessary */
    if (limit > prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        while (new_cap < limit) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_tbl = tmp;
        prime_cap = new_cap;
    }

    for (uint64_t cand = prime_limit + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        uint64_t r = isqrt_u64(cand);
        for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= r; ++i) {
            if (cand % prime_tbl[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) prime_tbl[prime_cnt++] = cand;
    }
    prime_limit = limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* base case a(0) = 1 */
    if (n == 0) return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    uint64_t max_p = isqrt_u64(temp);
    ensure_primes(max_p);

    seq_t prod = 1;
    for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= temp; ++i) {
        uint64_t p = prime_tbl[i];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            prod *= b_of_prime_power(p, e);
        }
    }
    if (temp > 1) {               /* remaining prime factor larger than sqrt(n) */
        prod *= b_of_prime_power(temp, 1);
    }
    return prod * 2;              /* a(n) = 2 * b(n) */
}
