/*
 * A339183  Number of partitions of n into two parts such that the smaller
 *          part is a nonzero square.
 * https://oeis.org/A339183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n/2))
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the integer square root of a 64‑bit number using binary search. */
#include <stdint.h>

static inline seq_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (n >> 1) + 1;          /* sqrt(n) <= n/2 + 1 for n > 1 */
    idx_t best = 0;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq <= n)
        {
            best = mid;
            low  = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return (seq_t)best;
}

seq_t a(idx_t n)
{
    idx_t half = n >> 1;          /* floor(n/2) */
    return isqrt_u64(half);
}
