/*
 * A247327  Triangle read by rows: T(n,k) = sum of k-th row of n X n square
 *          filled with odd numbers 1 through 2*n^2-1 reading across rows
 *          left-to-right.
 * https://oeis.org/A247327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: let m be the smallest integer with m*(m+1)/2 >= n;
 * let k = n - (m-1)*m/2; a(n) = (2*k - 1) * m^2.
 * This is the flattened triangle of sums of rows of n×n squares
 * filled with successive odd numbers.
 */
seq_t a(idx_t n)
{
    /* Find m so that triangular number T(m)=m*(m+1)/2 >= n */
    idx_t low = 1;
    idx_t high = n;            /* upper bound; safe because T(n) >= n for n>=1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* compute mid*(mid+1)/2 safely using 128‑bit arithmetic */
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;  /* m is the row (square size) */

    /* previous triangular number: T(m-1) = m*(m-1)/2 */
    __int128 prev_tri = (__int128)m * (m - 1) / 2;
    idx_t k = n - (idx_t)prev_tri;  /* 1‑based row index within the m‑row */

    /* compute the answer using seq_t arithmetic */
    seq_t mm = (seq_t)m;
    seq_t kk = (seq_t)k;
    seq_t ans = ((seq_t)2 * kk - (seq_t)1) * (mm * mm);
    return ans;
}
