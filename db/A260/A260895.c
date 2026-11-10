/*
 * A260895  Number of primes in n-th row of triangle A077664.
 * https://oeis.org/A260895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A010051(A077664(n,k)), i.e. the number of
 *            primes among the n smallest integers > n that are coprime to n
 *            (computed by a direct scan)
 * Range:     u8 n <= 509, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 972 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t A260895_gcd_u64(uint64_t x, uint64_t y)
{
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t cur = (uint64_t)n + 1;
    uint64_t found = 0;
    uint64_t primes = 0;
    while (found < (uint64_t)n) {
        if (A260895_gcd_u64((uint64_t)n, cur) == 1) {
            ++found;
            if (is_prime((idx_t)cur)) ++primes;
        }
        ++cur;
    }
    return (seq_t)primes;
}
