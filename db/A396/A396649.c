/*
 * A396649  Array read by ascending antidiagonals: A(n, k) = (-1)^k*(n -
 *          3*k)*binomial(n+k-1, k) / n for n > 0, T(0, 0) = 1, and T(0, k) =
 *          (-1)^(k+1)*3 for k > 0.
 * https://oeis.org/A396649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = read by ascending antidiagonals of the array T(i,j) where
 *            T(i,j) = (-1)^j * (i - 3*j) * binomial(i+j-1, j) / i for i > 0,
 *            T(0,0) = 1, T(0,k) = (-1)^(k+1)*3 for k > 0. The diagonal order
 *            is by increasing i+j, and within a diagonal from largest row
 *            index i down to 0 (i.e. increasing column j)
 * Range:     i8 n <= 31, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root floor for unsigned 64‑bit */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, res = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && mid > n / mid)
            high = mid - 1;
        else
        {
            res = mid;
            low = mid + 1;
        }
    }
    return res;
}

/* multiplicative binomial coefficient for non‑negative integers
   (works for small n as required by the sequence) */
static inline seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find diagonal d with d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t t = isqrt_u64(8 * n + 1);           /* floor(sqrt(8n+1)) */
    idx_t d = (t - 1) >> 1;                   /* (t-1)/2 */

    /* position inside diagonal: offset from the top (row 0)   */
    idx_t offset = n - (d * (d + 1)) / 2;      /* 0 .. d */

    /* coordinates: go downwards inside the diagonal   */
    idx_t i = d - offset;   /* row index (i) */
    idx_t j = offset;       /* column index (j) */

    /* special case i = 0 */
    if (i == 0)
    {
        if (j == 0) return 1;
        return (j & 1) ? 3 : -3;   /* (-1)^(j+1)*3 */
    }

    seq_t bin = binomial_small(i + j - 1, j);
    seq_t factor = (seq_t)i - (seq_t)3 * (seq_t)j;

    seq_t val = bin * factor;
    if (j & 1) val = -val;          /* multiply by (-1)^j */
    val = val / (seq_t)i;            /* exact division */

    return val;
}
