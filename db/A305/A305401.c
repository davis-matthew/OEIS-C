/*
 * A305401  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          A(n,k) is Sum_{j=0..floor(n/2)}
 *          ((n-j)!/j!)*binomial(n-j,j)*k^(n-2*j).
 * https://oeis.org/A305401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 86, u64 n <= 184,
 *            u128 n <= 455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A305401: a(n) read by antidiagonals from the square array
 *  A(n,k) = k*n*A(n-1,k) + A(n-2,k)  (n>1),
 *  with A(0,k)=1 and A(1,k)=k.
 *  a(0)=A(0,0), a(1)=A(0,1), a(2)=A(1,0), …
 */
seq_t a(idx_t n)
{
    /* locate the antidiagonal (s) and offset (d) for index n */
    idx_t remaining = n;
    idx_t s = 0;                 /* diagonal sum n+k */
    while (remaining >= s + 1) { /* each diagonal has s+1 elements */
        remaining -= s + 1;
        ++s;
    }
    idx_t d = remaining;         /* n coordinate along the diagonal */
    idx_t row = d;               /* n in A(row,k) */
    idx_t col = s - d;           /* k in A(row,k) */

    /* compute A(row,col) by recurrence */
    if (row == 0)
        return (seq_t)1;
    if (row == 1)
        return (seq_t)col;

    seq_t prev2 = (seq_t)1;          /* A(0,col)   */
    seq_t prev1 = (seq_t)col;        /* A(1,col)   */
    for (idx_t i = 2; i <= row; ++i) {
        seq_t cur = (seq_t)col * (seq_t)i * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
