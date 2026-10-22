/*
 * A321105  One of the three successive approximations up to 13^n for 13-adic
 *          integer 5^(1/3). This is the 11 (mod 13) case (except for n = 0).
 * https://oeis.org/A321105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321105: for n>0, a(n) is the unique k≡11 (mod 13) with k³≡5 (mod 13ⁿ);
   a(0)=0.  Hensel lifting is used to compute successive approximations. */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t k = (seq_t)11;      /* current root modulo 13ⁱ, starts with 13¹ */
    seq_t m = (seq_t)13;      /* 13ⁱ   */

    for (idx_t i = 1; i < n; ++i) {
        /* m_next = 13ⁱ⁺¹ */
        seq_t m_next = m * (seq_t)13;

        /* k³ modulo 13ⁱ⁺¹ */
        seq_t k3 = mod_pow(k, (idx_t)3, m_next);

        /* (k³ – 5) modulo m_next, guaranteed to be < m_next */
        seq_t diff = k3 + m_next - (seq_t)5;
        if (diff >= m_next)
            diff -= m_next;

        /* q = (k³ – 5) / 13ⁱ  (0 .. 12) */
        seq_t q = diff / m;

        /* t ≡ –q * (3k²)⁻¹  (mod 13).  For k≡11 (mod 13), denom = 12, inv = 12 */
        seq_t t = ((seq_t)13 - q) % (seq_t)13; /* –q mod 13 */
        t = (t * (seq_t)12) % (seq_t)13;      /* t = –q·12 mod 13 */

        /* lift k to modulus 13ⁱ⁺¹ */
        k += t * m;
        m = m_next;
    }

    return k;
}
