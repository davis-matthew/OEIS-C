/*
 * A272532  Single bit representation of the sum of two sinusoids with periods
 *          2 and 2*sqrt(2).
 * https://oeis.org/A272532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if floor(n / sqrt(2)) is even, otherwise 0
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(n / sqrt(2)) using integer arithmetic:
 * We find the largest m such that 2*m*m <= n*n.
 * Then a(n) = 1 if m is even, else 0.
 */
seq_t a(idx_t n)
{
    /* n is at least 1 by definition of the sequence. */
    unsigned __int128 n2 = (unsigned __int128)n * (unsigned __int128)n;
    idx_t low = 0, high = n;  /* m always < n for n >= 1 */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 mid2 = (unsigned __int128)mid * (unsigned __int128)mid;
        unsigned __int128 val = 2 * mid2;
        if (val <= n2) {
            ans = mid;          /* mid is a candidate */
            low = mid + 1;      /* try a larger one */
        } else {
            if (mid == 0) break;/* avoid underflow */
            high = mid - 1;
        }
    }
    /* ans now holds floor(n / sqrt(2)) */
    return (seq_t)((ans & 1) == 0 ? 1 : 0);
}
