/*
 * A319443  Number of distinct Eisenstein primes in the factorization of n.
 * https://oeis.org/A319443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p∣n, p prime} (p ≡ 1 (mod 3) ? 2 : 1)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9805 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. 2 for each distinct prime factor ≡ 1 mod 3, otherwise 1) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static uint64_t *primes = NULL;   /* array of primes up to current limit   */
static size_t primes_cnt = 0;     /* number of primes in the array         */
static idx_t prime_limit = 0;     /* highest prime that has been generated  */

seq_t a(idx_t n)
{
    if (n < 2)                    /* a(1) = 0, handled naturally below */
        return 0;

    /* Ensure we have primes up to sqrt(n) for trial division. */
    idx_t limit = (idx_t)floor_sqrt((seq_t)n);
    if (limit > prime_limit) {
        free(primes);                       /* discard old array if any   */
        build_primes_upto(limit, &primes, &primes_cnt);
        prime_limit = limit;
    }

    uint64_t temp = (uint64_t)n;
    seq_t count = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > limit) break;
        if (temp % p == 0) {
            /* distinct prime factor found */
            if (p % 3 == 1)
                count += 2;
            else
                count += 1;
            while (temp % p == 0)
                temp /= p;
            limit = (idx_t)floor_sqrt((seq_t)temp);
        }
    }

    /* If a prime factor larger than sqrt(original n) remains. */
    if (temp > 1) {
        if (temp % 3 == 1)
            count += 2;
        else
            count += 1;
    }

    return count;
}
