/*
 * A323184  Numerators of rationals whose continued fraction representations
 *          show the prime factors of n (for n>1) in nondecreasing order.
 * https://oeis.org/A323184
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the continued fraction [0; p1, p2, …, pk]
 *            where p1 ≤ p2 ≤ … ≤ pk are the prime factors (with multiplicity)
 *            of n. The value of the fraction equals 1 / (p1 + 1/(p2 + 1/(… +
 *            1/ pk)…)). The function computes this numerator exactly
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 2. For smaller n return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Factor n and retrieve its prime factors (ascending) and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t num_factors = 0;
    factor_u64((uint64_t)n, primes, exps, &num_factors);

    /* Build a flattened list of prime factors with multiplicity. */
    size_t total_len = 0;
    for (size_t i = 0; i < num_factors; ++i)
        total_len += exps[i];

    /* total_len is small (max ~ 64 for 64‑bit n), so a stack array is fine. */
    seq_t coeffs[128];   /* safety margin */
    size_t pos = 0;
    for (size_t i = 0; i < num_factors; ++i) {
        for (size_t j = 0; j < exps[i]; ++j) {
            coeffs[pos++] = (seq_t)primes[i];
        }
    }

    /* Compute the value of the continued fraction y = [p1; p2; …; pk] */
    seq_t num = coeffs[total_len - 1];
    seq_t den = 1;

    for (size_t i = total_len - 1; i-- > 0;) {
        seq_t temp = num;
        num = coeffs[i] * num + den;
        den = temp;
    }

    /* The required numerator a(n) is the denominator of y. */
    return den;
}
