/*
 * A329199  a(n) = round(log_3(n)).
 * https://oeis.org/A329199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(log_3(n))
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t pow = 1;   /* 3^k, k starts at 0 */
    idx_t    k   = 0;   /* floor(log_3(n)) */

    /* find the largest power of 3 that does not exceed n */
    while (pow <= n / 3) {
        pow *= 3;
        ++k;
    }

    /* decide whether to round up: compare n^2 with 3 * pow^2
       uses 128‑bit arithmetic to avoid overflow            */
    unsigned __int128 A = (unsigned __int128)n * (unsigned __int128)n;
    unsigned __int128 B = (unsigned __int128)pow * (unsigned __int128)pow;

    if (A / B >= 3)
        ++k;            /* round up to k+1 */

    return (seq_t)k;
}
