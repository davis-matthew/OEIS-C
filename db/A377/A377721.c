/*
 * A377721  Numbers that are not of the form k(k+1)(2k+1)/6 + k.
 * https://oeis.org/A377721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + m if n > m(m+1)(2m+1)/6 * a(n) = n + m-1 otherwise *
 *            where m = floor((3n)^(1/3))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t icube_root_3_mul_u64(idx_t n)
{
    /* Compute floor(cuberoot(3*n)) using 128‑bit arithmetic to avoid overflow. */
    typedef unsigned __int128 u128;
    u128 t = (u128)3 * (u128)n;          /* 3*n might overflow 64‑bit, use 128‑bit */
    idx_t low = 0, high = 1;
    /* Find an upper bound for the cube root. */
    while ((u128)high * high * high <= t)
        high <<= 1;
    low = high >> 1;                     /* low < cubic root <= high */
    /* Binary search for the largest x with x^3 <= 3*n. */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        u128 mid3 = (u128)mid * mid * mid;
        if (mid3 <= t)                  /* mid is a candidate, search higher */
            low = mid + 1;
        else                            /* mid^3 too large, search lower */
            high = mid - 1;
    }
    return low - 1;                      /* low is first > root, so root = low-1 */
}

seq_t a(idx_t n)
{
    /* n is at least 1 according to the OEIS offset. */
    idx_t m = icube_root_3_mul_u64(n);
    seq_t ms = (seq_t)m;
    seq_t s  = ms * (ms + 1) * (2 * ms + 1) / 6;   /* m(m+1)(2m+1)/6  */
    seq_t nn = (seq_t)n;
    return (nn > s) ? nn + m : nn + m - 1;
}
