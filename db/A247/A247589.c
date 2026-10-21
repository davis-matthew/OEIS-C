/*
 * A247589  Number of integer-sided obtuse triangles with largest side n.
 * https://oeis.org/A247589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A247589: Number of integer‑sided obtuse triangles with largest side n */
/* Sequence is computed by a direct O(n) double loop with integer arithmetic. */

#include <stdint.h>

/* Integer square root of a non‑negative 64‑bit integer: floor(sqrt(x)) */
static inline seq_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return (seq_t)hi;
}

seq_t a(idx_t n)
{
    /* No obtuse triangles for n < 3 */
    if (n < 3)
        return 0;

    seq_t total = 0;
    idx_t b_start = (n >> 1) + 1;          /* floor(n/2)+1  */
    for (idx_t b = b_start; b <= n - 1; ++b) {
        /* Triangle inequality: a + b > n  =>  a >= n - b + 1 */
        idx_t a_min = (n - b) + 1;
        if (a_min == 0)                /* safety for n=1,2 */
            a_min = 1;

        /* Obtuse condition: a^2 + b^2 < n^2  =>  a^2 < n^2 - b^2 */
        idx_t nsq = n * n;
        idx_t bsq = b * b;
        idx_t M = nsq - bsq;           /* M > 0 because b <= n-1 */
        if (M <= 1)
            continue;                  /* no a can satisfy the inequality */
        M -= 1;                        /* now a^2 <= M-1 < M */

        seq_t a_max = isqrt_u64(M);     /* floor(sqrt(M-1)) */
        seq_t b_seq = (seq_t)b;
        if (a_max > b_seq)
            a_max = b_seq;             /* enforce a <= b */

        seq_t a_min_seq = (seq_t)a_min;
        if (a_max >= a_min_seq)
            total += (seq_t)(a_max - a_min_seq + 1);
    }
    return total;
}
