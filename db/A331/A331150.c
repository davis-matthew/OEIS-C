/*
 * A331150  Triangle read by rows: T(n,k) (n>=k>=1) = f(n,n-k+1) where f(n,k)
 *          = floor((n/k)*ceiling(n/k)).
 * https://oeis.org/A331150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((r/col)*ceil(r/col)), where r = smallest integer
 *            with r*(r+1)/2 >= n, k = n - r*(r-1)/2 (1 ≤ k ≤ r), col = r - k
 *            + 1. This follows from T(n,k) = f(n,n-k+1) and the triangle read
 *            by rows
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t r = 1;
    idx_t cum = 1;               /* triangular sum of lengths up to row r */
    while (cum < n) {
        r++;
        cum += r;                /* cum = r*(r+1)/2   avoiding overflow */
    }

    idx_t prev = cum - r;        /* length up to previous row    */
    idx_t k = n - prev;          /* column index (1‑based)       */
    idx_t col = r - k + 1;       /* parameter for f(r, col)      */

    seq_t s   = (seq_t)r / (seq_t)col;          /* floor(r/col) */
    seq_t rem = (seq_t)r % (seq_t)col;          /* r mod col    */
    seq_t t   = ((seq_t)r + (seq_t)col - 1) / (seq_t)col; /* ceil(r/col) */

    seq_t val = s * t + (rem * t) / col;        /* floor product */
    return val;
}
