/*
 * A384845  Array read by antidiagonals: T(n,m) is the number of spanning
 *          trees in the n X m rook graph K_n X K_m.
 * https://oeis.org/A384845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is the n‑th entry in the antidiagonal
 *            traversal of the table T(n,m) = n^(n-2) * m^(m-2) *
 *            (n+m)^((n-1)*(m-1))
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 23, u64 n <= 39, u128 n <= 70,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but safe for 0 */

    /* Find the antidiagonal (s) containing the n‑th term.
       cumulative(s) = (s-1)*s/2 gives the total number of terms up to
       diagonal s (with s = i+j).  We locate the smallest s with
       cumulative(s) >= n.  For n ≤ 10 000, a simple loop is fast.      */
    idx_t s       = 2;                    /* first diagonal has s = 2 */
    idx_t cum     = 1;                    /* (s-1)*s/2 for s=2 */
    while (cum < n)
    {
        ++s;
        cum = (s - 1) * s / 2;           /* integer arithmetic, safe */
    }

    /* Position within the diagonal where the n-th term resides.
       prev = cumulative(s-1) = (s-2)*(s-1)/2   */
    idx_t prev    = (s - 2) * (s - 1) / 2;
    idx_t k       = n - prev - 1;         /* 0‑based offset inside diagonal */

    idx_t i = 1 + k;                      /* row index */
    idx_t j = s - i;                      /* column index */

    /* Compute T(i,j) = i^(i-2) * j^(j-2) * (i+j)^((i-1)*(j-1))   */
    seq_t result = 1;

    if (i >= 2)
        result *= big_pow((seq_t)i, (seq_t)(i - 2));

    if (j >= 2)
        result *= big_pow((seq_t)j, (seq_t)(j - 2));

    seq_t sum   = (seq_t)i + (seq_t)j;
    idx_t exp   = (i - 1) * (j - 1);
    if (exp != 0)
        result *= big_pow(sum, (seq_t)exp);

    return result;
}
