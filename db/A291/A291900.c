/*
 * A291900  Sum of the divisors of 24*n - 1, divided by 24, minus n.
 * https://oeis.org/A291900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(24*n - 1)/24 - n (sigma(k) is the sum of divisors
 *            of k)
 * Range:     u8 n <= 8, u16 n <= 3142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We factor 24*n - 1 by simple trial division.
   For n up to 10 000 this costs at most 490 iterations, which is fast
   and works for all n that fit in idx_t. */

#include <stdint.h>

/* Compute the total of divisors of 24*n - 1 and return the sequence value. */
seq_t a(idx_t n) {
    /* Compute 24*n-1 using only 64‑bit arithmetic first.  */
    const uint64_t m = 24ULL * (uint64_t)n - 1ULL;

    /* Factorisation */
    uint64_t temp = m;
    seq_t sigma = 1;                 /* product of divisor sums for each prime */

    /* Trial division by all integers up to sqrt(temp). */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* Compute the sum of powers 1 + p + p² + … + pᵉ for this prime. */
            seq_t power = 1;
            seq_t sum   = 1;
            do {
                temp /= p;
                power = power * (seq_t)p;
                sum += power;
            } while (temp % p == 0);
            sigma = sigma * sum;
        }
    }

    /* If anything remains, it is a prime factor with exponent 1. */
    if (temp > 1) {
        sigma = sigma * ((seq_t)1 + (seq_t)temp);
    }

    /* Now compute a(n) = sigma/24 - n. */
    return sigma / (seq_t)24 - (seq_t)n;
}
