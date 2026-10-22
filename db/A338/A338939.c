/*
 * A338939  a(n) is the number of partitions n = a + b such that a*b is a
 *          perfect square.
 * https://oeis.org/A338939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A for even n, a(n) = (A-1)/2 for odd n, where A = ∏_{p≡1
 *            (mod 4)} (2·e_p + 1), e_p is the exponent of p in n. 2 is
 *            ignored in this product
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the number of partitions n=a+b such that a·b is a perfect square. */
seq_t a(idx_t n)
{
    /* Work with unsigned 64‑bit arithmetic for factorisation. */
    uint64_t tmp = (uint64_t)n;
    seq_t A = 1;

    /* Remove all factors of 2 – they do not contribute to A. */
    while ((tmp & 1) == 0)
        tmp >>= 1;

    /* trial division by odd numbers up to sqrt(tmp) */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            /* p ≡ 1 (mod 4) contributes to the product A */
            if ((p & 3) == 1) {
                A *= (seq_t)(2 * exp + 1);
            }
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        /* exp = 1 for the remaining prime */
        if ((tmp & 3) == 1) {
            A *= (seq_t)3;
        }
    }

    /* final value depends on parity of n */
    if ((n & 1) == 0)
        return A;
    else
        return (A - 1) / 2;
}
