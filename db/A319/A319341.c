/*
 * A319341  a(n) = A000010(n) - A173557(n).
 * https://oeis.org/A319341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n) – ∏{p odd prime | n} (p – 1) where φ(n) is Euler’s
 *            totient function. The product over an empty set equals 1, so for
 *            a pure power of two the formula becomes a(n) = φ(n) – 1. This
 *            implementation factors n by trial division
 * Range:     u8 n <= 287, u16 n <= 9288, u32 n <= 9288, u64 n <= 9288,
 *            u128 n <= 9288, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1, guard against 0 */
        return 0;

    uint64_t temp = n;
    seq_t phi = (seq_t)n;      /* start φ(n) = n */
    seq_t prod = 1;           /* product of (p–1) over odd primes */

    /* Handle factor 2 separately (does not contribute to the product) */
    if (temp % 2 == 0) {
        phi -= phi / 2;       /* φ(n) *= (1 – 1/2)  */
        while (temp % 2 == 0)
            temp /= 2;        /* remove all 2’s */
    }

    /* Trial division by odd numbers up to √temp */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            phi -= phi / (seq_t)p;          /* φ(n) *= (1 – 1/p) */
            prod = prod * (seq_t)(p - 1);   /* multiply by (p – 1) */
            while (temp % p == 0)
                temp /= p;                  /* remove all copies of p */
        }
    }

    /* Any remaining prime factor > √original n */
    if (temp > 1) {              /* temp is an odd prime here */
        phi -= phi / (seq_t)temp;
        prod = prod * (seq_t)(temp - 1);
    }

    return phi - prod;
}
