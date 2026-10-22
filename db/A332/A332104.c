/*
 * A332104  Triangle read by rows in which row n >= 0 lists numbers from 0 to
 *          n starting at floor(n/2) and using alternatively larger
 *          respectively smaller numbers than the values used so far.
 * https://oeis.org/A332104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2) and c = n -
 *            r*(r+1)/2; T(r,c) = (r+c)/2 if r+c even else floor((r-c)/2)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n) {
        if (high > ((idx_t)-1) / 2) {     /* avoid overflow */
            high = ((idx_t)-1);
            break;
        }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t start = (__int128)r * (r + 1) / 2;
    idx_t c = n - start;

    /* Compute T(r,c) */
    if (((r + c) & 1) == 0) {
        return (seq_t)(r + c) / 2;
    } else {
        return (seq_t)(r - c) / 2;
    }
}
