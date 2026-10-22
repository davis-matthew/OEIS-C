/*
 * A371892  G.f. A(x) satisfies A(x) = 1 + x/A(x) * (1 + A(x)^4).
 * https://oeis.org/A371892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^(n-1) * (1/n) * Σ_{k=0..n} binomial(n,k) * *
 *            binomial(2*n-4*k-2, n-1) (with binomial defined for non‑negative
 *            * arguments; for negative top values the generalized binomial *
 *            coefficient is used via the second range)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* a(0) = 1 */

    seq_t sum_pos = 0;   /* positive contribution */
    seq_t sum_neg = 0;   /* negative contribution */

    const idx_t r = n - 1;              /* fixed lower index in binomials */
    const idx_t k1_max = (n - 1) / 4;   /* range where 2*n-4*k-2 >= n-1   */
    const idx_t k2_min = (n - 1) / 2 + 1; /* range where 4*k-n < 0 giving negative top  */

    seq_t Cnk = 1;   /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* --- range 1: k <= k1_max (positive sign) --- */
        if (k <= k1_max)
        {
            idx_t top1 = 2 * n - 4 * k - 2;      /* top of first binomial */
            seq_t term = Cnk * binomial(top1, r);
            sum_pos += term;
        }
        /* --- range 2: k >= k2_min (generalized binomial) --- */
        else if (k >= k2_min)
        {
            idx_t top2 = 4 * k - n;              /* bottom of generalized binomial */
            seq_t alt = binomial(top2, r);
            seq_t term = Cnk * alt;
            if ((n - 1) & 1)       /* (-1)^(n-1) = -1 => negative contribution */
                sum_neg += term;
            else
                sum_pos += term;
        }

        /* --- update C(n,k) for next k (if any) --- */
        if (k < n)
        {
            /* C(n,k+1) = C(n,k) * (n-k)/(k+1) */
            Cnk = Cnk * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    /* absolute value of the signed sum */
    seq_t diff = (sum_pos >= sum_neg) ? (sum_pos - sum_neg)
                                      : (sum_neg - sum_pos);

    /* divide by n; the division is exact */
    return diff / (seq_t)n;
}
