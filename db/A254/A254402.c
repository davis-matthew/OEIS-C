/*
 * A254402  a(n) = floor(a(n-1)^(3/2)), a(1) = 3.
 * https://oeis.org/A254402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(a(n-1)^(3/2)), a(1) = 3
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Integer square root for unsigned __int128 (used only when seq_t is ≤ 8 bytes). */
static seq_t sqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = x + 1, mid, res = 0;

    while (low < high) {
        mid = low + (high - low) / 2;
        if (mid * mid <= x) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined before offset */
    seq_t cur = (seq_t)3;          /* a(1) = 3 */
    if (n == 1)
        return cur;

    /* Iterate to the requested index. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next;
        if (sizeof(seq_t) <= 8) {          /* built‑in type: use 128‑bit arithmetic */
            unsigned __int128 x = (unsigned __int128)cur;
            unsigned __int128 cube = x * x * x;   /* fits in 128‑bit up to the last 64‑bit term */
            next = sqrt128(cube);
        } else {                           /* arbitrary‑precision type */
            seq_t cube = cur * cur;
            cube = cube * cur;
            next = floor_sqrt(cube);
        }
        cur = next;
    }
    return cur;
}
