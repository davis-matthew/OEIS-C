/*
 * A399645  Expansion of g^9/(7-6*g)^4, where g = 1+x*g^7 is the g.f. of
 *          A002296.
 * https://oeis.org/A399645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1) * ( binomial(7*n+6,n) + sum_{k=0}^{n} 6^{\,n-k} *
 *            binomial(7*n+7,k) )) / 2 The implementation avoids repeated
 *            binomial evaluations by computing the binomial coefficients
 *            iteratively and the powers of 6 incrementally. It works for any
 *            integer type seq_t and idx_t supplied by the build
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ----- compute binomial(7*n+6, n) ----- */
    idx_t MN = 7*n + 6;                     /* upper index for the first binomial */
    seq_t bin1 = 1;                         /* C(MN,0) */
    for (idx_t i = 1; i <= n; ++i) {
        /* bin1 = bin1 * (MN - (i-1)) / i */
        bin1 = (bin1 * (MN - (i-1))) / i;
    }

    /* ----- compute the sum S = Σ_{k=0}^n 6^{n-k} * C(7*n+7, k) ----- */
    idx_t NK = 7*n + 7;                     /* upper index for the second binomial */
    seq_t bin2 = 1;                         /* C(NK,0) */
    for (idx_t i = 1; i <= n; ++i) {
        /* bin2 = bin2 * (NK - (i-1)) / i = C(NK,i) */
        bin2 = (bin2 * (NK - (i-1))) / i;
    }

    seq_t sum = 0;                          /* Σ term */
    seq_t pow6 = 1;                         /* 6^{n-k} for current k, starting at k=n */

    idx_t k = n;
    while (1) {
        sum = sum + pow6 * bin2;           /* add current term */
        if (k == 0) break;                 /* finished all k */
        /* prepare for next k-1 */
        pow6 = pow6 * 6;                   /* increment power of 6 */
        /* update bin2: C(NK,k-1) = C(NK,k) * k / (NK - k + 1) */
        bin2 = (bin2 * k) / (NK - k + 1);
        --k;
    }

    /* ----- assemble the final result ----- */
    seq_t temp = ((seq_t)n + 1) * (bin1 + sum);   /* numerator */
    seq_t result = temp / 2;                      /* exact division by 2 */

    return result;
}
