/*
 * A339001  a(n) = (-1)^n * Sum_{k=0..n} (-n)^k * binomial(n,k) * Catalan(k).
 * https://oeis.org/A339001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n * Sum_{k=0..n} (-n)^k * binomial(n,k) * Catalan(k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    seq_t pow_n_k = 1;          /* n^k */
    seq_t nseq   = (seq_t)n;    /* convert n to seq_t once */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = pow_n_k;                     /* n^k */
        term *= binomial(n, k);                   /* * C(n,k) */
        term *= catalan(k);                       /* * Catalan(k) */

        /* add or subtract according to parity of (n + k) */
        if (((n + k) & 1ULL) == 0)
            sum_pos += term;
        else
            sum_neg += term;

        if (k == n)                /* last step: no further power needed */
            break;

        pow_n_k *= nseq;           /* prepare n^(k+1) for next iteration */
    }

    return sum_pos - sum_neg;      /* a(n) is non‑negative for all n */
}
