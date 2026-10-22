/*
 * A360163  a(n) is the sum of the square roots of the divisors of n that are
 *          odd squares.
 * https://oeis.org/A360163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n, d odd square} sqrt(d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow_u64.h"

seq_t a(idx_t n)
{
    /* a(0) is outside the domain; return 0 for safety. */
    if (n == 0)
        return 0;

    /* Result starts as 1 (contribution from divisor 1). */
    seq_t result = (seq_t)1;

    /* Work on the odd part of n. */
    uint64_t odd = (uint64_t)n;
    while ((odd & 1ULL) == 0ULL)
        odd >>= 1;

    /* Trial‑division factorisation of the odd part. */
    for (uint64_t p = 3; p <= odd / p; p += 2) {
        if (odd % p != 0)
            continue;

        unsigned exp = 0;
        while (odd % p == 0) {
            odd /= p;
            exp++;
        }

        unsigned half = exp / 2;          /* floor(e/2) */
        seq_t pow_val = seq_pow_u64(p, half + 1); /* p^(half+1) */
        seq_t term = (pow_val - 1) / ((seq_t)(p - 1)); /* (p^{half+1}-1)/(p-1) */

        result *= term;
    }

    /* Remaining factor (if any) is a prime > sqrt(original n). */
    if (odd > 1) {
        /* exponent = 1 => term = 1, no effect on the product */
        /* Multiply explicitly for consistency. */
        seq_t term = 1;
        result *= term;
    }

    return result;
}
