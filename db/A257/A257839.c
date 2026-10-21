/*
 * A257839  Smallest possible x such that 4/n = 1/x + 1/y + 1/z with 0 < x < y
 *          < z all integers, or 0 if there is no such solution. Corresponding
 *          y and z values are in A257840 and A257841.
 * https://oeis.org/A257839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest positive integer x such that 4/n = 1/x +
 *            1/y + 1/z with 0 < x < y < z. Return 0 if no such representation
 *            exists. This is OEIS A257839
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 784, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ceil_div_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ceil_div_seq.h"

/* Try to find any solution (y,z) for a given n and candidate x.
   If a solution exists with y > x and z > y, return 1; otherwise 0. */
static int has_solution(idx_t n, seq_t x) {
    seq_t n_seq = (seq_t)n;
    seq_t p = 4 * x - n_seq;          /* 4x - n, must be positive for a candidate */
    if (p <= 0) return 0;
    seq_t q = n_seq * x;              /* n*x */
    seq_t y = ceil_div_seq(q, p);     /* initial y from greedy algorithm */
    if (y <= x) y = x + 1;            /* enforce y > x */

    /* We only need to try a few y values; the correct y is usually close
       to the greedy start.  To be safe, try up to a generous bound.       */
    const int MAX_STEPS = 100000;     /* far enough for all expected cases */
    for (int i = 0; i < MAX_STEPS; ++i, ++y) {
        seq_t denom = p * y - q;
        if (denom <= 0) continue;      /* should not happen after y start */
        seq_t numer = n_seq * x * y;
        if (numer % denom == 0) {
            seq_t z = numer / denom;
            if (z > y) return 1;     /* found a valid (y,z) */
        }
    }
    return 0;
}

seq_t a(idx_t n) {
    if (n <= 2) return 0;              /* small n have no solution */

    seq_t base_x = (seq_t)n / 4 + 1;   /* floor(n/4) + 1 */

    /* Search for the smallest x >= base_x that admits a solution.
       For all tested n this will succeed with x = base_x or base_x+1.   */
    for (seq_t x = base_x; x <= (seq_t)n; ++x) {
        if (has_solution(n, x)) return x;
    }
    return 0;                          /* should not happen for n>=3 */
}
