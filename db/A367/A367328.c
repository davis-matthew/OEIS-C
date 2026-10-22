/*
 * A367328  The x-coordinate of the point where x + y = n, x and y are
 *          integers and x/y is as close as possible to e.
 * https://oeis.org/A367328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the integer x ∈ [0, n‑1] that makes x/(n‑x) closest to e.
 * It can be found as the nearest integer to n*e/(1+e), i.e. floor or ceil
 * whichever yields the smaller |x/(n‑x) – e|.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset starts at 1, but guard anyway */

    /* high‑precision const for e */
    const long double e = 2.718281828459045235360287471352662497757L;

    /* real value of n*e/(1+e) */
    long double real_val = ((long double)n) * e / (1.0L + e);
    idx_t x_floor = (idx_t)floorl(real_val);

    /* clamp to valid range [0, n‑1] */
    if (x_floor > n - 1) x_floor = n - 1;
    if (x_floor > n)     x_floor = n - 1;

    /* compute error for floor candidate */
    idx_t best = x_floor;
    long double best_err = fabsl(((long double)x_floor) /
                                  ((long double)(n - x_floor)) - e);

    /* check ceil candidate if it is valid */
    if (x_floor + 1 < n) {
        idx_t x2 = x_floor + 1;
        long double err2 = fabsl(((long double)x2) /
                                 ((long double)(n - x2)) - e);
        if (err2 < best_err) {
            best = x2;
            best_err = err2;
        }
    }

    return (seq_t)best;
}
