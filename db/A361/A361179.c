/*
 * A361179  a(n) = sigma(n)^4.
 * https://oeis.org/A361179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(n)^4
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 107, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = (σ(n))^4 where σ(n) is the sum of all positive divisors of n.
   The function works for any seq_t type that supports unsigned arithmetic
   (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128, or arbitrary
   precision).  It performs prime factorisation of n by trial division,
   computes σ(n) multiplicatively, and finally raises the result to the
   fourth power. */

seq_t a(idx_t n)
{
    /* Sequence starts at index 1.  For n == 0 we return 0. */
    if (n == 0u)
        return 0u;

    uint64_t m = (uint64_t)n;          /* work with a non‑negative integer */
    seq_t sigma = 1u;                  /* product of σ(p^e) for each prime */

    /* Trial division over all possible prime factors up to sqrt(m). */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0u)
            continue;

        /* Count the exponent of prime p in m. */
        uint64_t exp = 0;
        while (m % p == 0u) {
            m /= p;
            ++exp;
        }

        /* Compute σ(p^exp) = 1 + p + p^2 + ... + p^exp. */
        seq_t term = 1u;          /* starts with p^0 = 1 */
        seq_t pow   = 1u;          /* current power of p */
        for (uint64_t i = 0; i < exp; ++i) {
            pow  *= (seq_t)p;
            term += pow;
        }
        sigma *= term;            /* multiply into the total product */
    }

    /* If a prime factor greater than sqrt(n) remains. */
    if (m > 1u) {
        seq_t term = 1u + (seq_t)m;   /* σ(m^1) = 1 + m */
        sigma *= term;
    }

    /* Compute sigma^4: (sigma^2)^2. */
    seq_t sq = sigma * sigma;
    seq_t res = sq * sq;
    return res;
}
