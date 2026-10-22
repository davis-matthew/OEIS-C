/*
 * A305466  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          A(n,k) is Sum_{j=0..floor(n/2)}
 *          ((n-j)!/j!)*binomial(n-j,j)*k^(n-2*j)*(-1)^j.
 * https://oeis.org/A305466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 41, i32 n <= 85, i64 n <= 184,
 *            i128 n <= 428, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   A(0,k) = 1,          A(1,k) = k,
   A(n,k) = k*n*A(n-1,k) - A(n-2,k)    for n > 1.
   The sequence a(n) is obtained by reading the array A over
   antidiagonals: a(n) = A(i,j) where
     d      = floor( ( sqrt(8*n+1) - 1 ) / 2 )
     i      = n - d*(d+1)/2
     j      = d - i   (i and j are non‑negative integers). */

seq_t a(idx_t n)
{
    /* Find d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while (( (__int128)(d + 1) * (d + 2) ) / 2 <= n)
        ++d;

    /* offset inside the diagonal */
    idx_t sum_before = (idx_t)(( (__int128)d * (d + 1) ) / 2);
    idx_t offset     = n - sum_before;
    idx_t i          = offset;           /* row index   */
    idx_t j          = d - offset;        /* column index */

    /* Special cases */
    if (i == 0) return (seq_t)1;          /* A(0,j) = 1 */
    if (i == 1) return (seq_t)j;          /* A(1,j) = j */

    /* Iteratively compute A(i,j) using the recurrence */
    seq_t prev2 = (seq_t)1;               /* A(0,j) */
    seq_t prev1 = (seq_t)j;               /* A(1,j) */
    seq_t cur;

    for (idx_t row = 2; row <= i; ++row)
    {
        cur = (seq_t)j * (seq_t)row * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
