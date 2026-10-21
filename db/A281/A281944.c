/*
 * A281944  Triangle read by rows: T(n,k) (n>=1, 3<=k<=n+2) is the number of
 *          k-sequences of balls colored with n colors such that exactly two
 *          balls are of a color seen previously in the sequence.
 * https://oeis.org/A281944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by rows from the triangle T(n,k) * with T(n,k) =
 *            (binomial(k,3)+3*binomial(k,4)) * n!/(n+2-k)!
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 54, u64 n <= 168, u128 n <= 493,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small binomial coefficient: choose k from n (k <= 4). */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* safety; offset starts at 1 */

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t r = lo;                           /* row number */

    /* Offset of the current entry within its row. */
    idx_t prev   = (r - 1) * r / 2;
    idx_t offset = n - prev - 1;            /* 0‑based */
    idx_t k = 3 + offset;                   /* column index */

    /* Compute the coefficient C(k,3)+3*C(k,4). */
    seq_t coeff = binom_small(k, 3);
    coeff += (seq_t)3 * binom_small(k, 4);

    /* Compute the factorial ratio n!/(n+2-k)! = product_{t=n+2-k+1}^{n} t. */
    idx_t start = r + 2 - k + 1;            /* n+2-k+1  */
    if (start < 1)
        start = 1;                         /* safety; not needed for valid k */
    seq_t prod = 1;
    for (idx_t t = start; t <= r; ++t)
        prod *= (seq_t)t;

    return coeff * prod;
}
