/*
 * A337192  Triangular array read by rows. T(n,k) is the number of elements of
 *          rank k in the order complex of the poset P = [n] X [n], n=0, k=0
 *          or n>0, 0<=k<=2n-1.
 * https://oeis.org/A337192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) reads the triangular array T(n,k) by rows, where T(n,0) = 1
 *            for all n ≥ 0, and for 1 ≤ k ≤ 2n−1 T(n,k) = Σ_{i=0}^{k−1}
 *            (−1)^i * C(k−1,i) * [ C(n+k−1−i , k−i) ]² (Here C denotes the
 *            binomial coefficient.) The function below implements this
 *            formula using the supplied helper ‘binom’
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* row 0, single element */

    /* Find the row index r such that 1 + r(r+1) > n ≥ 1 + (r−1)r. */
    idx_t r = 0;
    while (1 + r * (r + 1) <= n) ++r;
    idx_t row = r;

    /* Start of this row in the flattened sequence. */
    idx_t start = (row == 0) ? 0 : (1 + (row - 1) * row);
    idx_t k = n - start;              /* position within the row */

    if (k == 0) return (seq_t)1;       /* empty chain */

    /* Inclusion–exclusion to forbid consecutive equal pairs. */
    seq_t pos = 0;     /* sum of terms with even i */
    seq_t neg = 0;     /* sum of terms with odd i */

    for (idx_t i = 0; i <= k - 1; ++i) {
        seq_t cki = binom(k - 1, i);                       /* C(k-1,i)   */
        seq_t bin = binom(row + k - 1 - i, k - i);          /* C(row+k-1-i, k-i) */
        seq_t term = cki * bin * bin;                      /* C * (C)^2  */
        if (i & 1) {
            neg += term;
        } else {
            pos += term;
        }
    }
    return pos - neg;
}
