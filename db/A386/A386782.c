/*
 * A386782  a(n) = n^3*sigma_8(n).
 * https://oeis.org/A386782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^3 * σ₈(n) (σ₈ is the sum of the 8th powers of the
 *            divisors of n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 3183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Takes n (0‑based index).  For n=0 returns 0.  For n>0 computes n³*σ₈(n).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* use trial division to factor n */
    idx_t m = n;
    seq_t sigma = (seq_t)1;              /* running product of σ₈(pᵉ) terms */

    for (idx_t p = 2; (p <= m / p); ++p) {
        if (m % p != 0) continue;

        /* exponent of prime p in n */
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* compute σ₈(pᵉ) = Σ_{k=0}^{e} p^{8k} */
        seq_t term   = (seq_t)1;         /* p^{8k} */
        seq_t sum    = (seq_t)1;         /* accumulates Σ p^{8k} */
        seq_t p8     = (seq_t)1;         /* p⁸ */

        /* compute p⁸ once */
        for (int i = 0; i < 8; ++i) {
            p8 *= (seq_t)p;
        }

        /* multiply term by p⁸ e times, accumulating the sum */
        for (int i = 0; i < e; ++i) {
            term *= p8;
            sum  += term;
        }

        sigma *= sum;
    }

    /* any remaining factor >1 is a prime with exponent 1 */
    if (m > 1) {
        seq_t p8 = (seq_t)1;
        for (int i = 0; i < 8; ++i) {
            p8 *= (seq_t)m;
        }
        sigma *= ((seq_t)1 + p8);
    }

    /* compute n³ and multiply by σ₈(n) */
    seq_t n_seq = (seq_t)n;
    seq_t n_cub = n_seq * n_seq * n_seq;
    return n_cub * sigma;
}
