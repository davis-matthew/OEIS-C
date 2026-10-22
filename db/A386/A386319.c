/*
 * A386319  Triangle read by rows where row n is the start, corner and end
 *          vertex numbers of a triangular spiral with n sides on a triangular
 *          grid, starting from 1 and working inwards (0 <= k <= n).
 * https://oeis.org/A386319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, * T(r,0)=1, T(r,k)= k*(2*r-k+1)/2 (k>=1)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    while (((__int128)high * (high + 1)) / 2 <= n) {
        high <<= 1;
        if (high == 0) {        /* overflow guard, should not happen for n <= 2^64-1 */
            high = (idx_t)-1;
            break;
        }
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 offset_mid = (__int128)mid * (mid + 1) / 2;
        if (offset_mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;
    idx_t k = n - (idx_t)(((__int128)r * (r + 1)) / 2);
    if (k == 0)
        return (seq_t)1;
    __int128 numerator = (__int128)k * (( (__int128)2 * r - (__int128)k + 1));
    return (seq_t)(numerator / 2);
}
