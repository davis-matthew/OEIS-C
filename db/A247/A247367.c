/*
 * A247367  Number of ways to write n as a sum of a square and a nonsquare.
 * https://oeis.org/A247367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of squares s (0 ≤ s ≤ n) such that n – s is NOT a
 *            perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses a binary‑search integer square‑root routine that
    works for all n ≤ 2^64 – 1.  The algorithm is O(√n) per call, well
    below 2 s for n ≈ 10 000. */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n.  */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else {
            if (mid == 0)   /* avoid underflow */
                break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Count all squares s ≤ n (including 0). */
    idx_t sqrt_n = isqrt_u64(n);
    idx_t count = sqrt_n + 1;          /* squares: 0^2, 1^2, …, sqrt_n^2 */

    /* Subtract those where the complement n – s is also a square. */
    for (idx_t i = 0; i <= sqrt_n; ++i) {
        idx_t s = i * i;
        idx_t t = n - s;
        idx_t r = isqrt_u64(t);
        if (r * r == t)
            --count;
    }
    return (seq_t)count;
}
