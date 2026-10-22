/*
 * A374369  Triangle T(n, k), n > 0, k = 0..n-1, read by rows; T(n, k) is the
 *          least m such that n and k differ modulo m.
 * https://oeis.org/A374369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = A007978( r - ( n - 1 - r*(r-1)/2 ) )
   r = ceil( (sqrt(8*n+1)-1) / 2 )                               */
seq_t a(idx_t n)
{
    /* find row r: smallest r with r*(r+1)/2 >= n          */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t r = lo;                                            /* row number */

    unsigned __int128 prev = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - (idx_t)prev - 1;   /* column index (0 .. r-1) */

    idx_t d = r - k;                     /* difference n - k   */

    seq_t m = 2;
    while ((seq_t)d % m == 0)
        ++m;                             /* smallest m>=2 not
                                             dividing d                */
    return m;
}
