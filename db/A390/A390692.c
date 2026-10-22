/*
 * A390692  a(n) is the smallest positive integer such that a(n) * n is a
 *          Loeschian number (A003136).
 * https://oeis.org/A390692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements A390692: a(n) = product of primes p|n with p ≡ 2 (mod 3) that
 * occur to an odd exponent in the prime factorisation of n.
 * This is multiplicative with a(p^e) = p if e is odd and p ≡ 2 (mod 3),
 * otherwise a(p^e) = 1.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1)
        return (seq_t)1;

    seq_t result = 1;
    idx_t temp = n;
    int count;

    /* factor 2 separately */
    count = 0;
    while ((temp & 1ULL) == 0) {   /* temp % 2 == 0 */
        temp >>= 1;                /* temp /= 2 */
        ++count;
    }
    if ((count & 1) && (2 % 3) == 2)
        result *= (seq_t)2;

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            count = 0;
            while (temp % p == 0) {
                temp /= p;
                ++count;
            }
            if ((count & 1) && (p % 3) == 2)
                result *= (seq_t)p;
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        if ((temp % 3) == 2)
            result *= (seq_t)temp;
    }

    return result;
}
