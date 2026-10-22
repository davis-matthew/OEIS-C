/*
 * A352031  Sum of the cubes of the odd proper divisors of n.
 * https://oeis.org/A352031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n, d odd} d^3 * a(n) = sigma_3(odd_part(n)) -
 *            (n^3 if n odd, else 0)
 * Range:     u8 n <= 13, u16 n <= 81, u32 n <= 3209, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;   /* a(1) = 0, a(0) undefined by OEIS offset */
    }

    /* Extract odd part of n */
    idx_t temp = n;
    while ((temp & 1) == 0) {
        temp >>= 1;
    }

    seq_t result = (seq_t)1;

    /* Factorization of the odd part */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t p3 = pow_uint((uint64_t)p, (idx_t)3);
            seq_t cur = (seq_t)1;
            seq_t term = (seq_t)1;
            for (int i = 0; i < exp; i++) {
                cur *= p3;     /* p^{3*(i+1)} */
                term += cur;   /* add p^{3*i} for sum of powers */
            }
            result *= term;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        seq_t p3 = pow_uint((uint64_t)temp, (idx_t)3);
        seq_t cur = (seq_t)1;
        seq_t term = (seq_t)1;
        cur *= p3;        /* exponent 1 */
        term += cur;
        result *= term;
    }

    /* If original n is odd, subtract n^3 to exclude the divisor n itself */
    if (n & 1) {
        result -= pow_uint((uint64_t)n, (idx_t)3);
    }

    return result;
}
