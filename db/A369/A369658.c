/*
 * A369658  a(n) = 1 if n is not multiple of 3, but its arithmetic derivative
 *          is, otherwise 0.
 * https://oeis.org/A369658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is not divisible by 3 and the arithmetic
 *            derivative of n is divisible by 3; otherwise 0
 * Range:     u8 n <= 9917, u16 n <= 9917, u32 n <= 9917, u64 n <= 9917,
 *            u128 n <= 9917, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A369658
#include <stdint.h>

/* Computes a(n) as defined above.  The arithmetic derivative d(n) is
 * d(n) = n * Σ (e_i / p_i) where n = ∏ p_i^e_i.  For n not divisible by 3,
 * d(n) ≡ 0 (mod 3) iff Σ e_i * inv(p_i) ≡ 0 (mod 3), where inv(p_i) is
 * the multiplicative inverse of p_i modulo 3.  Since n is not divisible by 3,
 * the factor n itself never contributes a factor of 3 modulo 3.  The inverse
 * is simply p_i (mod 3) because 1⁻¹≡1 and 2⁻¹≡2 (mod 3).  The function factorises
 * n with elementary trial division; this is fast enough for the required
 * range (n ≲ 10 000) and scales to larger 64‑bit values.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined by OEIS list */
    if (n % 3 == 0) return (seq_t)0;   /* multiple of 3 ⇒ 0 */

    /* For n == 1 the derivative is 0 ⇒ 1. */
    if (n == 1) return (seq_t)1;

    uint64_t m = n;
    int sum_mod3 = 0;

    /* factor 2 */
    int e = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }
    if (e) {
        /* 2 ≡ 2 (mod 3), its inverse is also 2. */
        sum_mod3 = (sum_mod3 + (e % 3) * 2) % 3;
    }

    /* trial division by odd numbers */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            int inv = (p % 3 == 1) ? 1 : 2;   /* 3 does not divide n here */
            sum_mod3 = (sum_mod3 + (e % 3) * inv) % 3;
        }
    }

    /* remaining prime factor > sqrt(original n) if any */
    if (m > 1) {
        int inv = (m % 3 == 1) ? 1 : 2;
        sum_mod3 = (sum_mod3 + inv) % 3;  /* exponent is 1 */
    }

    return (seq_t)(sum_mod3 == 0 ? 1 : 0);
}
