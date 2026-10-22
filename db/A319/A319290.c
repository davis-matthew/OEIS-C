/*
 * A319290  The y coordinates of the shell enumeration of N X N where N = {0,
 *          1, 2, ...}(A319514).
 * https://oeis.org/A319290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A319290: y coordinate of the shell enumeration of N×N
 *
 * For n ≥ 0 let
 *     k = ⌊√n⌋,
 *     t = n – k².
 * If k is even:
 *     a(n) = (t ≤ k) ? t : k.
 * If k is odd:
 *     a(n) = (t ≤ k) ? k : (2·k – t).
 */
#include <stdint.h>

/* Integer square‑root of an unsigned 64‑bit integer.
 * Returns ⌊√n⌋.  The routine uses a binary search with 128‑bit multiplication
 * to avoid overflow.  It is inlined to avoid function call overhead. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq <  n) low  = mid + 1;
        else          high = mid - 1;
    }
    return high; /* floor(√n) */
}

seq_t a(idx_t n)
{
    idx_t k = isqrt_u64(n);
    idx_t t = n - k * k;       /* n lies in the interval [k², (k+1)²-1] */
    if ((k & 1) == 0) {        /* k is even */
        if (t <= k) return (seq_t)t;
        else        return (seq_t)k;
    } else {                   /* k is odd */
        if (t <= k) return (seq_t)k;
        else        return (seq_t)(2 * k - t);
    }
}
