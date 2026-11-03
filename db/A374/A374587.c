/*
 * A374587  The maximum exponent in the prime factorization of the numbers
 *          that are not coprime to the maximum exponent in their prime
 *          factorization.
 * https://oeis.org/A374587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9655 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374587 – maximum exponent in the prime factorization of numbers
   that are not coprime to that maximum exponent.  The implementation
   scans positive integers m in order, keeps the n‑th number m with
   gcd(m, max_exponent(m)) > 1, and returns its maximum exponent.  */

#include <stdbool.h>
#include "../utils/gcd.h"

static inline seq_t max_exponent(idx_t n)
{
    seq_t maxe = 0;
    idx_t temp = n;
    idx_t exp = 0;

    /* factor out powers of 2 */
    while ((temp & 1ULL) == 0) {
        exp++;
        temp >>= 1;
    }
    if (exp > 0)
        maxe = exp;

    /* factor odd primes up to sqrt(temp) */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                exp++;
                temp /= p;
            }
            if (exp > maxe)
                maxe = exp;
        }
    }

    if (temp > 1) {          /* remaining prime factor with exponent 1 */
        if (maxe < 1)
            maxe = 1;
    }
    return maxe;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;

    while (1) {
        m++;
        seq_t e = max_exponent(m);
        if (e <= 1)            /* gcd(m,1)=1, skip */
            continue;

        if (gcd(m, e) > 1) {
            count++;
            if (count == n)
                return e;
        }
    }
}
