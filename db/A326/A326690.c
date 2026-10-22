/*
 * A326690  Denominator of the fraction (Sum_{prime p | n} 1/p - 1/n).
 * https://oeis.org/A326690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (∑_{p|n} 1/p - 1/n)
 * Range:     u8 n <= 20, u16 n <= 885, u32 n <= 9115, u64 n <= 9115,
 *            u128 n <= 9115, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /*  Special case n = 1  */
    if (n == 1) return 1;

    uint64_t temp = (uint64_t)n;
    uint64_t primes_arr[64];
    size_t cnt = 0;

    /* factor out 2 */
    if ((temp & 1) == 0) {
        primes_arr[cnt++] = 2;
        while ((temp & 1) == 0)
            temp >>= 1;
    }
    /* trial division for odd factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            primes_arr[cnt++] = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    /* remaining prime factor, if any */
    if (temp > 1)
        primes_arr[cnt++] = temp;

    /* product P of distinct prime factors */
    seq_t P = 1;
    for (size_t i = 0; i < cnt; ++i)
        P *= (seq_t)primes_arr[i];

    /* sum_{p|n} P/p  */
    seq_t sum_terms = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum_terms += P / (seq_t)primes_arr[i];

    /* numerator and denominator of the fraction */
    seq_t numerator = sum_terms * (seq_t)n - P;
    seq_t denom     = P * (seq_t)n;

    /* reduce fraction */
    seq_t g = gcd(numerator, denom);
    return denom / g;
}
