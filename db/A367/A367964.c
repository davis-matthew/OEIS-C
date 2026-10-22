/*
 * A367964  Triangle of 2-parameter triangular numbers, read by rows. T(n, k)
 *          = (n*(n + 1) + k*(k + 1)) / 2.
 * https://oeis.org/A367964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, c) where r = floor((sqrt(8*n + 1) - 1) / 2), c = n -
 *            r*(r + 1)/2, T(r, c) = (r*(r + 1) + c*(c + 1)) / 2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find r such that r*(r + 1)/2 <= n < (r + 1)*(r + 2)/2.
       Binary search for O(log n). */
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        idx_t t = mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t r = high;                 /* r is the largest with triangular(r) <= n */
    idx_t c = n - (r * (r + 1) / 2);  /* position inside the row */

    /* Compute T(r, c) in seq_t. */
    seq_t tr = (seq_t)r * ((seq_t)r + 1) / 2;
    seq_t tc = (seq_t)c * ((seq_t)c + 1) / 2;
    return tr + tc;
}
