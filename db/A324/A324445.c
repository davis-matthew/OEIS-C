/*
 * A324445  Number of labeled cyclic chord diagrams with n chords such that
 *          the minimal chord length equals one.
 * https://oeis.org/A324445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001147(n) - A003436(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Σ_{k=1}^{n} (-1)^{k+1} * (2n)/(2n-k) * C(2n-k,k) * (2n-2k-1)!!          */
#include "../utils/binom.h"
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 by definition. */
    if (n == 1)
        return (seq_t)1;

    /* m = 2n */
    idx_t m = 2 * n;

    /* Compute (2n-1)!! once.  Use odd_fact_n for later division. */
    seq_t odd_fact_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        odd_fact_n = odd_fact_n * ((seq_t)2 * i - (seq_t)1);

    /* cur will hold (2n-2k-1)!! for the current k value. */
    seq_t cur = odd_fact_n;

    seq_t sum_pos = (seq_t)0;   /* terms with k odd (positive) */
    seq_t sum_neg = (seq_t)0;   /* terms with k even (negative) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update cur to (2n-2k-1)!! from previous value. */
        idx_t odd_factor = (idx_t)(2 * (n - k + 1) - 1);
        cur = cur / ((seq_t)odd_factor);

        /* Compute m_k = (2n)/(2n-k) * C(2n-k, k). */
        seq_t mk = (seq_t)m * binom((idx_t)(m - k), k);
        mk = mk / ((seq_t)(m - k));

        /* term = m_k * (2n-2k-1)!!. */
        seq_t term = mk * cur;

        if (k & 1U)   /* k odd: positive contribution */
            sum_pos += term;
        else          /* k even: negative contribution */
            sum_neg += term;
    }

    /* sum_pos should be >= sum_neg for all n >= 1, so the subtraction
       produces a non‑negative result. */
    return sum_pos - sum_neg;
}
