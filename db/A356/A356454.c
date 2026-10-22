/*
 * A356454  Numbers k such that 4*k^2 is not in A014567; complement of
 *          A356451.
 * https://oeis.org/A356454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that gcd(k, sigma(4*k^2)) > 1
 *            (sigma = sum of divisors, A000203). This function searches k =
 *            1,2,3,... and returns the n-th term of that sequence
 * Range:     u8 none, u16 n <= 10, u32 n <= 4850, u64 n <= 9141,
 *            u128 n <= 9141, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t sigma_prime_power(uint64_t p, unsigned int exp)
/* sum_{i=0}^{exp} p^i  */
{
    unsigned __int128 term = 1;
    unsigned __int128 sum  = 1;
    for (unsigned int i = 1; i <= exp; ++i) {
        term *= p;
        sum  += term;
    }
    return (seq_t)sum;
}

seq_t a(idx_t n)
{
    idx_t count = 0;

    for (idx_t k = 1; ; ++k) {
        /* factor exponent of 2 in k */
        unsigned int e2 = 0;
        uint64_t tmp = (uint64_t)k;
        while ((tmp & 1ULL) == 0) {
            ++e2;
            tmp >>= 1;
        }

        /* sigma(4*k^2) part for the factor 2 */
        unsigned int exp2 = 2 + 2 * e2;              /* exponent of 2 in 4*k^2 */
        unsigned __int128 pow2 = ((unsigned __int128)1 << (exp2 + 1)); /* 2^(exp2+1) */
        seq_t sigma = (seq_t)(pow2 - 1);             /* sigma(2^exp2) */

        /* factor remaining odd part */
        uint64_t m = tmp;                            /* odd part of k */
        if (m > 1) {
            for (uint64_t p = 3; p * p <= m; p += 2) {
                if (m % p == 0) {
                    unsigned int ep = 0;
                    while (m % p == 0) {
                        ++ep;
                        m /= p;
                    }
                    sigma *= sigma_prime_power(p, 2 * ep);
                }
            }
            if (m > 1) {
                /* remaining prime factor with exponent 1 */
                sigma *= sigma_prime_power(m, 2);
            }
        }

        if (gcd((seq_t)k, sigma) != (seq_t)1) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
