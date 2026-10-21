/*
 * A242389  2^n minus the sum of the proper divisors of n.
 * https://oeis.org/A242389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - (σ(n) - n) where σ(n) is the sum of all positive
 *            divisors of n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n using the provided helper. */
    seq_t pow2n = pow2(n);

    /* Compute σ(n) via prime factorisation. */
    seq_t sigma = 1;                 /* Initial product. */
    uint64_t temp = (uint64_t)n;     /* Work on a mutable copy of n. */

    for (idx_t p = 2; (uint64_t)p * (uint64_t)p <= temp; ++p) {
        if (temp % p == 0) {
            /* Count the exponent e of prime p in n. */
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* Compute (p^(e+1) - 1) / (p - 1). */
            seq_t numerator = 1;
            for (idx_t i = 0; i < e + 1; ++i)
                numerator *= (seq_t)p;          /* Multiply p (e+1) times. */
            numerator -= 1;
            seq_t denominator = (seq_t)p - 1;
            seq_t term = numerator / denominator;

            /* Update σ(n). */
            sigma *= term;
        }
    }

    /* If a prime factor > sqrt(n) remains, it occurs exactly once. */
    if (temp > 1) {
        seq_t term = (seq_t)temp + 1;  /* For prime p: (p^2 - 1)/(p - 1) = p + 1. */
        sigma *= term;
    }

    /* sum of proper divisors = σ(n) - n */
    seq_t proper = sigma - (seq_t)n;

    /* Final result. */
    return pow2n - proper;
}
