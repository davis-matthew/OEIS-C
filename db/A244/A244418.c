/*
 * A244418  Triangle read by rows T(n,m) = n*m +(n-1)*(m-1), for n >= m >= 1.
 * https://oeis.org/A244418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = n*m + (n-1)*(m-1) where m = n - T(n-1) and T(k) =
 *            k*(k+1)/2 is the k-th triangular number. The index n starts at 1
 * Range:     u8 n <= 44, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t k)
{
    /* Handle the (unlikely) case of index 0 gracefully. */
    if (k == 0)
        return 0;

    /* Find the row number 'n' such that n*(n+1)/2 >= k. */
    idx_t low  = 1;
    idx_t high = k;           /* n can never exceed k */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = (seq_t)mid * (mid + 1) / 2;
        if (tri >= (seq_t)k)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t n = low;

    /* Compute column index 'm' within that row. */
    seq_t prev_tri = (seq_t)(n - 1) * n / 2;   /* sum of elements before row n */
    seq_t m = (seq_t)k - prev_tri;             /* 1‑based column index */

    /* Evaluate the entry using the triangle formula. */
    seq_t val = (seq_t)n * m + (seq_t)(n - 1) * (m - 1);
    return val;
}
