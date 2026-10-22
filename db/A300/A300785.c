/*
 * A300785  Triangle read by rows: T(n,k) = 140*k^3*(n-k)^3 - 14*k*(n-k) + 1;
 *          n >= 0, 0 <= k <= n.
 * https://oeis.org/A300785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 140*k^3*(n-k)^3 - 14*k*(n-k) + 1 where r = largest
 *            integer with r*(r+1)/2 <= n and k = n - r*(r+1)/2 (i.e. a(n) is
 *            the (n+1)-st entry of the triangle T(n,k) read by rows.)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 680, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r and column k corresponding to the flat index n  */
    idx_t r   = 0;      /* current row number */
    idx_t tri = 0;      /* triangular number r*(r+1)/2 */

    /* Increment r until the next triangular number would exceed n   */
    while (tri + r + 1 <= n) {
        tri += r + 1;   /* tri becomes (r+1)*(r+2)/2 after r++ */
        r++;
    }

    idx_t k = n - tri;  /* column within the current row (0 <= k <= r) */

    /* Compute the triangle entry T(r,k) = 140*k^3*(r-k)^3 - 14*k*(r-k) + 1 */
    seq_t kk   = (seq_t)k;
    seq_t nk   = (seq_t)(r - k);
    seq_t kk3  = kk * kk * kk;     /* k^3   */
    seq_t nk3  = nk * nk * nk;     /* (r-k)^3  */
    seq_t prod = kk3 * nk3;        /* k^3 * (r-k)^3  */

    seq_t result = (seq_t)140 * prod - (seq_t)14 * kk * nk + (seq_t)1;
    return result;
}
