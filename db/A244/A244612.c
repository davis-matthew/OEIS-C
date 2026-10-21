/*
 * A244612  a(n) = 1 if n is a square, -1 if n is six times a square, 0 if n <
 *          1.
 * https://oeis.org/A244612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n)=1 if n is a perfect square, a(n)=-1 if n=6*k^2 for some k,
 *            a(n)=0 otherwise
 * Range:     i8 n <= 9776, i16 n <= 9776, i32 n <= 9776, i64 n <= 9776,
 *            i128 n <= 9776, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root (floor) for 64‑bit unsigned integers. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* Return the A244612 sequence value for index n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* defined to be 0 for n < 1, not needed but harmless */

    /* Check if n is a perfect square. */
    idx_t k = floor_isqrt_u64(n);
    if ((idx_t)k * k == n)
        return (seq_t)1;

    /* Check if n is six times a perfect square. */
    if (n % 6 == 0) {
        idx_t m = n / 6;
        idx_t k2 = floor_isqrt_u64(m);
        if ((idx_t)k2 * k2 == m)
            return (seq_t)-1;
    }

    return 0;
}
