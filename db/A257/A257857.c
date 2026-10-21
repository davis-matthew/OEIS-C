/*
 * A257857  Sequentially filled binary triangle rotated 180 degrees and then
 *          superimposed and added to the original triangle.
 * https://oeis.org/A257857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = value of the flattened “sequentially filled binary
 *            triangle” rotated 180°, superimposed, and added to the original
 *            triangle. The triangle entries T(r,c) are * T(r,c) = 1 if r is
 *            even, * T(r,c) = 2 if r is odd and ((r+1)/2 + c) is even, *
 *            T(r,c) = 0 if r is odd and ((r+1)/2 + c) is odd. The...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that the n‑th element lies in it.
       Intervals are 1 … 1, 2 … 3, 4 … 6, …, i.e. cumulative
       sums R(r)=r*(r+1)/2.  */
    idx_t lo = 1, hi = n;                /* answer lies in [lo,hi] */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= (__int128)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;   /* row index (1‑based) */

    /* Column index within the row.  */
    __int128 prev_tri = (__int128)(r - 1) * r / 2;
    idx_t c = (idx_t)((__int128)n - prev_tri);  /* 1‑based column */

    /* Compute the triangle value according to the definition.  */
    seq_t val;
    if ((r & 1) == 0)          /* even row */
        val = 1;
    else
    {
        idx_t m = (r + 1) / 2;
        val = ((m + c) & 1) == 0 ? 2 : 0;
    }
    return val;
}
