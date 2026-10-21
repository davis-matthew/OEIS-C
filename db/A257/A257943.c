/*
 * A257943  Array A read by upward antidiagonals in which the entry in row n
 *          and column k is defined by A(n,k) = (1 + 3^(n-1)*(2*k - 1))/2, n,k
 *          >= 1.
 * https://oeis.org/A257943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (1 + 3^(row-1)*(2*col-1))/2, where row and col are
 *            obtained by reading the array A(n,k) = (1 + 3^(n-1)*(2*k-1))/2
 *            along upward anti‑diagonals. For an index n (1‑based) let d =
 *            smallest integer with n ≤ d*(d-1)/2 (anti‑diagonal number) m = n
 *            - (d-2)*(d-1)/2 (position within that diagonal) r...
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal d (≥2) that contains the index n.
       Use binary search on d with 128‑bit arithmetic to avoid overflow. */
    idx_t lo = 2;
    idx_t hi = n + 1;          /* hi is always large enough */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid - 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;              /* anti‑diagonal number */

    /* Compute position m inside diagonal d */
    unsigned __int128 prev_tri128 = (unsigned __int128)(d - 2) * (d - 1) / 2;
    idx_t m = n - (idx_t)prev_tri128;      /* 1‑based index within diagonal */

    /* Determine the array coordinates (row, col) */
    idx_t row = d - m;         /* row index (n ≥ 1) */
    idx_t col = d - row;       /* column index (k ≥ 1) */

    /* Compute 3^(row-1) using repeated multiplication. */
    seq_t power = 1;
    idx_t exp = row - 1;
    for (idx_t i = 0; i < exp; ++i)
        power = power * (seq_t)3;

    seq_t term = (seq_t)(2 * col - 1);
    seq_t val  = ((seq_t)1 + power * term) / 2;
    return val;
}
