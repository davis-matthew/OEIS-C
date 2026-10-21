/*
 * A295126  Denominator of Sum_{d|n} mu(n/d)/d, where mu is the Möbius
 *          function A008683.
 * https://oeis.org/A295126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, frac, easy
 * Formula:   a(n) = n / gcd( n , ∏_{p|n} (p-1) ) where the product runs over
 *            distinct prime divisors of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mul64.h"

/* Return the denomin­ator of Σ_{d|n} μ(n/d)/d   (n ≥ 1). */
seq_t a(idx_t n) {
    /* Handle the trivial case n = 1 directly. */
    if (n == 1) {
        return (seq_t)1;
    }

    uint64_t orig = (uint64_t)n;        /* original n, unchanged later */
    uint64_t temp = orig;              /* value to factor */
    uint64_t prod_mod = 1 % orig;      /* product of (p-1) modulo orig */

    /* Factor 2 separately for a slight speed gain. */
    if ((temp & 1ULL) == 0) {
        prod_mod = mul64(prod_mod, (uint64_t)1, orig);   /* (2-1) = 1 */
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
        }
    }

    /* Trial division by odd numbers. */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            prod_mod = mul64(prod_mod, (p - 1ULL) % orig, orig);
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }

    /* If a prime factor remains > 1, include it. */
    if (temp > 1) {
        prod_mod = mul64(prod_mod, (temp - 1ULL) % orig, orig);
    }

    /* Compute gcd and the final denominator. */
    seq_t g     = gcd((seq_t)orig, (seq_t)prod_mod);
    seq_t denom = (seq_t)orig / g;
    return denom;
}
