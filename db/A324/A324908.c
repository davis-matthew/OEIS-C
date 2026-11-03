/*
 * A324908  a(n) = 1 if n is an odd number which is not a square, 0 otherwise.
 * https://oeis.org/A324908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and not a perfect square, 0 otherwise
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9976 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t integer_sqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0)          // even numbers
        return 0;

    idx_t r = integer_sqrt(n);    // floor(sqrt(n))
    if (r * r == n)               // perfect square
        return 0;

    return 1;                     // odd non‑square
}
