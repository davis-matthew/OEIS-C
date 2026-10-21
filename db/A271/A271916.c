/*
 * A271916  Array read by antidiagonals: T(m,n) (m>=1, n>=1) = f(m,n) if m <=
 *          n or f(n,m) if n < m, where f(m,n) = m*(m-1)*(3*n-m-1)/6.
 * https://oeis.org/A271916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) read by antidiagonals of T(m,n) where T(m,n) =
 *            f(min(m,n),max(m,n)) and f(m,n) = m*(m-1)*(3*n-m-1)/6
 * Range:     u8 n <= 59, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 >= n. */
    idx_t d = 0;
    idx_t tri = 0;            /* triangular number T(d) = d*(d+1)/2 */
    while (tri < n) {
        d++;
        tri += d;             /* accumulate to avoid recomputation */
    }

    /* triPrev is T(d-1) = tri - d. */
    idx_t triPrev = tri - d;

    /* offset within the current diagonal (0‑based). */
    idx_t offset = n - triPrev - 1;

    /* Coordinates of the element. */
    idx_t m = offset + 1;     /* row */
    idx_t ncol = d - offset;  /* column */

    /* T(m,ncol) uses the smaller coordinate as the first argument. */
    idx_t r = m;
    idx_t c = ncol;
    if (r > c) {
        idx_t tmp = r;
        r = c;
        c = tmp;
    }

    /* Compute f(r,c) = r*(r-1)*(3*c-r-1)/6. */
    seq_t R = (seq_t)r;
    seq_t C = (seq_t)c;

    seq_t val = R * (R - 1);
    val = val * (3 * C - R - 1);
    val = val / 6;

    return val;
}
