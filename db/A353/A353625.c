/*
 * A353625  a(n) = 1 if n is divisible by the squares of two distinct primes,
 *          otherwise 0.
 * https://oeis.org/A353625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is divisible by squares of two distinct primes,
 *            otherwise 0
 * Range:     u8 n <= 63, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 131 terms through n = 9638 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* we need prime list up to floor(sqrt(n)) */
    idx_t limit = (idx_t)floor_sqrt((seq_t)n);
    /* build primes up to the limit (rebuild each time – cheap for n ≤ 10000) */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(limit, &primes, &primes_cnt);

    uint64_t temp = (uint64_t)n;
    size_t count = 0;          /* number of distinct primes with exponent ≥ 2 */

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i)
    {
        uint64_t p = primes[i];
        uint32_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp >= 2) {
            ++count;
            if (count >= 2) {
                /* free allocated primes array before returning */
                free(primes);
                return (seq_t)1;
            }
        }
    }

    /* Remaining prime factor > sqrt(original n) has exponent 1 */
    /* cleanup */
    free(primes);
    return (seq_t)0;
}
