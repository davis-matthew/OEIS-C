/*
 * A363990  a(n) = Sum_{k = 0..n}
 *          (-4)^(n-k)*binomial(n,k)*binomial(4*n+2*k,2*k)* binomial(2*k,k).
 * https://oeis.org/A363990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n}
 *            (-4)^(n-k)*binomial(n,k)*binomial(4*n+2*k,2*k)*binomial(2*k,k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t pos = 0;          /* sum of positive terms  */
    seq_t neg = 0;          /* sum of negative terms  */
    seq_t c1 = 1;           /* binomial(n,k)           */
    seq_t c2 = 1;           /* binomial(4*n+2*k,2*k)   */
    seq_t c3 = 1;           /* binomial(2*k,k)         */
    seq_t pow_factor = 1;   /* 4^(n-k) for current k   */

    /* pre‑compute 4^n */
    for (idx_t i = 0; i < n; ++i) pow_factor *= 4;

    for (idx_t k = 0; k <= n; ++k) {
        /* term magnitude = c1 * c2 * c3 * 4^(n-k) */
        seq_t term = c1;
        term *= c2;
        term *= c3;
        term *= pow_factor;

        /* add according to the sign of (-1)^(n-k) */
        if (((n - k) & 1) == 0)
            pos += term;
        else
            neg += term;

        if (k == n) break;   /* no more terms after the last one */

        /* update binomial factors for next k+1 */
        c1 = c1 * (n - k) / (k + 1);

        idx_t A = 4 * n + 2 * k;     /* A = 4n + 2k */
        seq_t num  = ((seq_t)A + 2) * ((seq_t)A + 1);
        seq_t den  = ((seq_t)(2 * k + 2)) * ((seq_t)(2 * k + 1));
        c2 = c2 * num / den;

        seq_t num3 = ((seq_t)(2 * k + 2)) * ((seq_t)(2 * k + 1));
        seq_t den3 = ((seq_t)(k + 1)) * ((seq_t)(k + 1));
        c3 = c3 * num3 / den3;

        /* advance power of 4: go from 4^(n-k) to 4^(n-(k+1)) */
        pow_factor /= 4;
    }

    seq_t result = pos;
    result -= neg;
    return result;
}
