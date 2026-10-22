/*
 * A357959  a(n) = 5*A005259(n-1) + 2*A005258(n).
 * https://oeis.org/A357959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*A005259(n-1) + 2*A005258(n) where A005259(m) =
 *            Σ_{k=0..m} binom(m,k)^2 * binom(m+k,k)^2 A005258(n) = Σ_{k=0..n}
 *            binom(n,k)^2 * binom(n+k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small_uint.h"

seq_t a(idx_t n)
{
    /* --- sum for A005259(n-1) --- */
    seq_t sumA = 0;
    if (n >= 1) {
        idx_t m = n - 1;                   /* m = n-1  */
        seq_t binom_m      = 1;            /* C(m,0)   */
        seq_t binom_m_plus = 1;            /* C(m+0,0) */

        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = binom_m * binom_m * binom_m_plus * binom_m_plus;
            sumA += term;

            if (k == m)
                break;

            /* next k : k+1  */
            binom_m      = mul_small_uint(binom_m,      (m - k))
                           / ((seq_t)(k + 1));
            binom_m_plus = mul_small_uint(binom_m_plus, (m + k + 1))
                           / ((seq_t)(k + 1));
        }
    }

    /* --- sum for A005258(n) --- */
    seq_t sumB = 0;
    {
        seq_t binom_n   = 1;                     /* C(n,0)   */
        seq_t binom_npq = 1;                     /* C(n+0,0) */

        for (idx_t k = 0; k <= n; ++k) {
            seq_t term = binom_n * binom_n * binom_npq;
            sumB += term;

            if (k == n)
                break;

            /* next k : k+1  */
            binom_n   = mul_small_uint(binom_n,   (n - k))
                       / ((seq_t)(k + 1));
            binom_npq = mul_small_uint(binom_npq, (n + k + 1))
                       / ((seq_t)(k + 1));
        }
    }

    /* final linear combination */
    return 5 * sumA + 2 * sumB;
}
