/*
 * A389671  a(n) is the number of distinct pairs of integer-sided
 *          quadrilaterals obtainable by making a single straight cut through
 *          an nXn square.
 * https://oeis.org/A389671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊n/2⌋ + Σ_{d=1}^{n-1, n^2 + d^2 is a square} ⌊(n - d)/2⌋
 * Range:     u8 n <= 119, u16 n <= 7559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of distinct pairs of integer‑sided quadrilaterals obtainable by
   a single straight cut through an n×n square. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(0) = a(1) = 0 */
    if (n < 2)
        return 0;

    /* rectangles */
    seq_t res = (seq_t)(n >> 1);   /* floor(n/2) */

    /* trapezoids */
    idx_t nsq = n * n;             /* n^2 as idx_t (fits 64‑bit for n≤10000) */

    for (idx_t u = 1; u * u <= nsq; ++u) {
        if (nsq % u == 0) {
            idx_t v = nsq / u;
            if (u < v && ((u + v) & 1) == 0) {          /* u and v have same parity */
                idx_t d = (v - u) >> 1;                 /* d = (v - u)/2, positive integer */
                if (d >= 1 && d < n) {
                    res += (seq_t)((n - d) >> 1);        /* ⌊(n - d)/2⌋ */
                }
            }
        }
    }

    return res;
}
