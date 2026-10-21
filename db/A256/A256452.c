/*
 * A256452  Number of integer solutions to n^2 = x^2 + y^2 with x>0, y>=0.
 * https://oeis.org/A256452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of integer solutions to n² = x² + y² with x>0,
 *            y≥0. * The sequence is multiplicative: for each prime p≡1 (mod
 *            4) with exponent e in * the factorisation of n, the contribution
 *            to a(n) is (2·e + 1); primes * congruent to 2 or 3 (mod 4)
 *            contribute a factor of 1. Thus * a(n) = ∏_{p≡1 (m...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the A256452 sequence value for a given n (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Sequence is defined for n ≥ 1; return 0 for safety. */
        return 0;
    }

    seq_t result = 1;               /* multiplicative identity */
    uint64_t m = (uint64_t)n;       /* work on a 64‑bit integer */

    /* handle factor 2 – it never contributes (p≡2 mod 4) */
    while ((m & 1) == 0) {
        m >>= 1;
    }

    /* trial division for odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (p % 4 == 1) {
                /* 2·exp + 1 fits easily in a seq_t for exp ≤ 63. */
                result *= (seq_t)(2 * exp + 1);
            }
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (m > 1 && m % 4 == 1) {
        /* exponent is 1 */
        result *= (seq_t)3;           /* 2·1 + 1 = 3 */
    }

    return result;
}
