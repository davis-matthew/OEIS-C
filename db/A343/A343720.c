/*
 * A343720  Triangle read by rows: T(n,k) = k^2 mod n for k = 0..n-1, n >= 1.
 * https://oeis.org/A343720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the term obtained by flattening the triangle T(n,k) =
 *            k^2 mod n (for n ≥ 1, k = 0 … n–1) row by row. The function
 *            finds the triangle row containing the (1‑based) index n by
 *            binary search on the triangular partial sums m*(m+1)/2
 * Range:     u8 n <= 169, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find smallest m such that m*(m+1)/2 ≥ n */
    idx_t low  = 1;
    idx_t high = n;            /* worst‑case bound, safe for any n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t m = low;                         /* row number (1‑based) */

    /* Number of elements before row m:  (m‑1)*m/2 */
    unsigned __int128 before = (unsigned __int128)(m - 1) * m / 2;
    idx_t k = n - (idx_t)before - 1;        /* position in row m, 0‑based */

    /* Compute k^2 mod m, using seq_t to keep type consistent */
    seq_t kk = (seq_t)k * (seq_t)k;         /* k^2 in seq_t */
    return kk % (seq_t)m;                   /* k^2 mod m */
}
