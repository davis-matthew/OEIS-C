/*
 * A399542  Array read by ascending antidiagonals: A(n,k) = n*k*(n^2 - k^2)/6,
 *          with n, k >= 0.
 * https://oeis.org/A399542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl, new
 * Formula:   a(n) is obtained by reading A(i,j)=i*j*(i^2−j^2)/6 down the
 *            antidiagonals of the array with i decreasing. For index n, let s
 *            = ⌊(√(8n+1)−1)/2⌋, offset = n − s(s+1)/2, i = s − offset, j =
 *            offset. Then a(n) = i*j*(i^2−j^2)/6
 * Range:     i8 n <= 56, i16 n <= 747, i32 n <= 9601, i64 n <= 9601,
 *            i128 n <= 9601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find s such that s(s+1)/2 ≤ n < (s+1)(s+2)/2 */
    idx_t low  = 0, high = 1;
    while (( (__int128)high * (high + 1) / 2 ) <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__int128)mid * (mid + 1) / 2 ) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t s      = low - 1;
    idx_t offset = n - ( (__int128)s * (s + 1) / 2 );
    idx_t i      = s - offset;
    idx_t j      = offset;

    __int128 prod = (__int128)i * j;
    __int128 diff = (__int128)i * i - (__int128)j * j;
    __int128 val  = prod * diff / 6;
    return (seq_t)val;
}
