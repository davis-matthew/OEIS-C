/*
 * A393841  a(n) is the number of n-digit terms in A393832.
 * https://oeis.org/A393841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{m=2..9} Sum_{k=2..m} binomial(m-1,k-1) * (
 *            n*binomial(n-1,n-k-1) - (n-1)*binomial(n-2,n-k-2) ) for n > 1
 *            a(1) = 1 The function uses the provided binom() utility for
 *            binomial coefficients
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 1 (a(0) is unused) */
        return (seq_t)1;

    seq_t total = (seq_t)0;
    seq_t n_seq  = (seq_t)n;
    seq_t n_minus_1 = (n > 0) ? (seq_t)(n-1) : (seq_t)0;
    seq_t n_minus_2 = (n > 1) ? (seq_t)(n-2) : (seq_t)0;

    for (idx_t m = 2; m <= 9; ++m) {
        for (idx_t k = 2; k <= m; ++k) {
            /* coefficient binomial(m-1, k-1) */
            seq_t coeff = binom(m-1, k-1);
            if (coeff == (seq_t)0)          /* skip zero coeffs (unlikely) */
                continue;

            seq_t add = (seq_t)0;

            /* Term 1 : n * binom(n-1, n-k-1) */
            if (k <= n-1) {                 /* ensure n-k-1 >= 0 */
                idx_t r1 = n - k - 1;       /* 0 <= r1 <= n-1 */
                seq_t term1 = n_seq * binom(n-1, r1);
                /* Term 2 : (n-1) * binom(n-2, n-k-2) */
                seq_t term2 = (seq_t)0;
                if (k <= n-2) {             /* ensure n-k-2 >= 0 */
                    idx_t r2 = n - k - 2;   /* 0 <= r2 <= n-2 */
                    term2 = n_minus_1 * binom(n-2, r2);
                }
                seq_t diff = term1 - term2; /* term1 >= term2 always */
                add = coeff * diff;
            }
            /* else term1 and term2 are zero; add stays zero */

            total += add;
        }
    }
    return total;
}
