/*
 * A248808  Irregular triangle read by rows: row n gives golden ratio base
 *          representation of Fibonacci number F_n.
 * https://oeis.org/A248808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, nice
 * Formula:   a(n) returns the nth term of the irregular triangle described in
 *            OEIS A248808. The triangle has rows indexed by r = 1, 2, …,
 *            where row r has L(r) = floor((r+1)/2) elements. For even rows (r
 *            = 2*k) the elements are T(2*k, m) = 2*(k‑1) – 4*m for m = 0 …
 *            k‑1. For odd rows (r = 2*k+1) the elements ar...
 * Range:     i8 n <= 2550, i16 n <= 2550, i32 n <= 2550, i64 n <= 2550,
 *            i128 n <= 2550, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x < 2^64 */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    /* Start with the highest power of two that fits in a 64‑bit value. */
    for (idx_t bit = ((idx_t)1) << 32; bit > 0; bit >>= 1)
    {
        idx_t next = r | bit;
        if (next <= x / next)
            r = next;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find k such that S(2*k) < n <= S(2*k+1) or n <= S(2*k). */
    idx_t k = isqrt_u64(n - 1);          /* floor(sqrt(n-1)) */

    idx_t S_even = k * (k + 1);          /* S(2*k) */

    /* Convert k to the signed type once; all further arithmetic is signed. */
    seq_t ks = (seq_t)k;

    if (n <= S_even)                    /* even row r = 2*k */
    {
        idx_t offset = n - k * k;        /* 1 .. k */
        idx_t m = offset - 1;           /* 0 .. k-1 */
        seq_t ms = (seq_t)m;
        return 2 * (ks - 1) - 4 * ms;   /* T(2k, m) */
    }
    else                                /* odd row r = 2*k+1 */
    {
        idx_t offset = n - S_even;      /* 1 .. k+1 */
        idx_t m = offset - 1;           /* 0 .. k */
        if (m < k)                      /* not the last element */
        {
            seq_t ms = (seq_t)m;
            return 2 * ks - 1 - 4 * ms; /* T(2k+1, m) */
        }
        else                            /* last element in the row */
        {
            return -2 * ks;             /* T(2k+1, k) */
        }
    }
}
