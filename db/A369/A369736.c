/*
 * A369736  Triangle read by rows. T(n, k) = 0 if n - k is odd otherwise if 4
 *          divides n - k then 1 otherwise -1.
 * https://oeis.org/A369736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = 0 if (r-k) is odd; otherwise T(r,k) = 1
 *            if 4 divides (r-k); else T(r,k) = -1
 * Range:     i8 n <= 8778, i16 n <= 8778, i32 n <= 8778, i64 n <= 8778,
 *            i128 n <= 8778, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
 
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0;
    idx_t high = n;
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        /* Compute mid*(mid+1)/2 without overflow using 128-bit. */
        __int128 prod = (__int128)mid * (__int128)(mid + 1) >> 1;
        if (prod <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    /* Compute k = n - r*(r+1)/2. */
    __int128 diag = (__int128)r * (__int128)(r + 1) >> 1;
    idx_t k = (idx_t)(n - (idx_t)diag);
    /* Compute difference d = r - k. */
    idx_t d = r - k;
    if (d & 1ULL) {
        return (seq_t)0;
    } else if ((d & 3ULL) == 0ULL) {
        return (seq_t)1;
    } else {
        return (seq_t)-1;
    }
}
