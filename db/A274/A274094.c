/*
 * A274094  a(0)=0; thereafter (-1)^(n+1)*n appears n times.
 * https://oeis.org/A274094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0; for n>0 let m be the smallest integer with m*(m+1)/2 ≥ n
 *            Then a(n) = (-1)^(m+1) * m (i.e., m appears m times with
 *            alternating sign)
 * Range:     i8 n <= 8256, i16 n <= 9580, i32 n <= 9580, i64 n <= 9580,
 *            i128 n <= 9580, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }

    idx_t m = lo;
    return (m & 1) ? (seq_t)m : -(seq_t)m;
}
