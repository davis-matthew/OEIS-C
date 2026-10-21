/*
 * A256232  Multiplicative with a(2^e) = 1-e, a(3^e) = 1, a(p^e) = e+1 if p>3.
 * https://oeis.org/A256232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = (1 – v₂(n)) * Π_{p>3} (v_p(n)+1) where v_p(n) is the
 *            exponent of prime p in n. This implements the multiplicative
 *            definition: a(2^e) = 1–e, a(3^e) = 1, a(p^e) = e+1 for p>3
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert to unsigned 64-bit for factorisation */
    uint64_t m = (uint64_t)n;

    /* Count exponent of 2 */
    unsigned e2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e2;
    }

    /* Remove all factors of 3 – they contribute a factor 1 */
    while (m % 3ULL == 0) {
        m /= 3ULL;
    }

    /* Now m has no factor 2 or 3 */
    seq_t prod = 1;                    /* product of (exponent+1) for p>3 */

    if (m > 1) {
        /* Factorise by trial division (sufficient for n up to ∼10⁴); */
        /* We step by 2 (since all even numbers already removed). */
        for (uint64_t p = 5; p * p <= m; p += 2) {
            if (m % p == 0) {
                unsigned exp = 0;
                while (m % p == 0) {
                    m /= p;
                    ++exp;
                }
                prod *= (seq_t)(exp + 1);
            }
        }
        /* Any remaining m is a prime > sqrt(original m).  Its exponent is 1. */
        if (m > 1) {
            prod *= (seq_t)2; /* (1+1) */
        }
    }

    /* Multiply by the 2‑component factor (1‑e2). */
    seq_t two_factor = (seq_t)1 - (seq_t)e2;
    return prod * two_factor;
}
