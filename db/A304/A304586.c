/*
 * A304586  A linear mapping a(n) = x + d*n of pairs of integers (x,d), where
 *          the pairs are enumerated by the counterclockwise square spiral (an
 *          axis-parallel number spiral) starting at 0.
 * https://oeis.org/A304586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = x + d*n where (x,d) are the coordinates of the
 *            counterclockwise square spiral (index 0 at (0,0))
 * Range:     i8 n <= 41, i16 n <= 1639, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Integer square root of a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0 || n == 1)
        return n;

    idx_t x = n;
    idx_t y = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* (x,d) = (0,0) */

    /* Determine ring k and offset t within that ring. */
    idx_t m = isqrt_u64(n);          /* floor(sqrt(n)) */
    idx_t k = (m + 1) / 2;           /* ring number (>=1) */

    /* Total elements up to the previous ring. */
    idx_t prev_total = (2 * k - 1);
    prev_total = prev_total * prev_total;   /* (2k-1)^2 */

    idx_t t = n - prev_total;        /* 0 <= t < 8k */

    seq_t kk = (seq_t)k;
    seq_t tt = (seq_t)t;

    seq_t x, d;                      /* coordinates of (x,d) */

    if (tt < 2 * kk) {               /* Right side: from (k, -(k‑1)) to (k, k). */
        x = kk;
        d = -(kk - 1) + tt;
    } else if (tt < 4 * kk) {        /* Top side: from (k‑1, k) to (-k, k). */
        seq_t s = tt - 2 * kk;
        x = kk - 1 - s;
        d = kk;
    } else if (tt < 6 * kk) {        /* Left side: from (-k, k‑1) to (-k, -k). */
        seq_t s = tt - 4 * kk;
        x = -kk;
        d = kk - 1 - s;
    } else {                         /* Bottom side: from (-k+1, -k) to (k, -k). */
        seq_t s = tt - 6 * kk;
        x = -kk + 1 + s;
        d = -kk;
    }

    /* Compute a(n) = x + d*n. */
    return x + d * (seq_t)n;
}
