/*
 * A361043  Array read by descending antidiagonals. A(n, k) is, if n > 0, the
 *          number of multiset permutations of {0, 1} of length n * k where
 *          the number of occurrences of 1 are multiples of n. A(0, k) = k +
 *          1.
 * https://oeis.org/A361043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(row, col) where the array A is defined by * A(0,k) = k
 *            + 1 * A(n,k) = Σ_{j=0..k} binomial(n*k, n*j) (n > 0) * The
 *            sequence a is the flattening of A by descending antidiagonals *
 *            (0,0), (0,1), (1,0), (0,2), (1,1), (2,0), … * so a(0) = A(0,0),
 *            a(1) = A(0,1), a(2) = A(1,0), …
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* n is the flat index */
    idx_t idx = n;

    /* locate the diagonal m that contains this index.
       Total terms before diagonal m is m*(m+1)/2. */
    long double t = sqrtl(8.0L * idx + 1.0L);
    idx_t m = (idx_t)((t - 1.0L) / 2.0L);

    /* number of terms before diagonal m */
    idx_t prev_terms = m * (m + 1) / 2;

    /* position within the diagonal: row (n in A(n,k)) and column (k) */
    idx_t row = idx - prev_terms;          /* n in A(n,k) */
    idx_t col = m - row;                   /* k in A(n,k) */

    /* Special case: n = 0 → A(0,k) = k + 1 */
    if (row == 0)
        return (seq_t)(col + 1);

    /* Compute A(row, col) for row > 0:
       A(row,col) = Σ_{j=0..col} binomial(row*col, row*j) */
    idx_t nk = row * col;
    seq_t sum = 0;
    for (idx_t j = 0; j <= col; ++j)
        sum += binomial(nk, row * j);

    return sum;
}
