/*
 * A325946  Maximum number of intercardinal adjacencies among all n-celled
 *          polyplets.
 * https://oeis.org/A325946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4n - ceil(sqrt(28n-12))
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;          /* hi^2 ~= 2^64 */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n<1, return 0 */

    idx_t t = 28 * n - 12;               /* argument for sqrt */
    idx_t fs = floor_sqrt_u64(t);        /* floor sqrt */
    idx_t ceil = (fs * fs == t) ? fs : fs + 1;
    return 4 * ((seq_t)n) - (seq_t)ceil;
}
