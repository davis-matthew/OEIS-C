/*
 * A386849  For n >= 0, a(n) is the least t >= 1 such that n^2 + 8*t is a
 *          square (A000290) where t is a triangular number (A000217) or a(n)
 *          = -1 if no such t exists.
 * https://oeis.org/A386849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) is the least triangular number t ≥ 1 such that n² + 8t is a
 *            square
 * Range:     i8 n <= 8, i16 n <= 33, i32 n <= 521, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Solve s² = n² + 4k(k+1) with t = k(k+1)/2, k ≥ 1. */
/* Using the transformation (s−y)(s+y) = n² − 1 where y = 2k+1, */
/* we enumerate all factor pairs of n² − 1, derive k, and keep the minimal t. */
/* Complexity: O(√(n²)) = O(n) per call, fast for n ≤ 10 000. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return -1;      /* a(0) = -1 */
    if (n == 1) return 1;       /* a(1) = 1, since 1² + 8·1 = 9 = 3² */

    /* n ≥ 2: compute m = n² − 1 */
    seq_t m = (seq_t)n * (seq_t)n - 1;   /* m > 0 */

    seq_t max_d = floor_sqrt(m);
    seq_t best_t = -1;   /* sentinel for “no solution found yet” */

    for (idx_t d = 1; d <= (idx_t)max_d; ++d) {
        if (m % d != 0) continue;      /* d is not a divisor */

        seq_t a_val = d;                 /* factor a = d */
        seq_t b_val = m / d;             /* factor b = m / d */

        /* parity check: a and b must have same parity */
        if (((a_val & 1) != (b_val & 1))) continue;

        seq_t diff = b_val - a_val;      /* diff = b − a ≥ 0 */

        /* need diff ≥ 6 and (diff − 2) divisible by 4 to get integer k ≥ 1 */
        if (diff < 6) continue;
        if (((diff - 2) & 3) != 0) continue;

        seq_t k = (diff - 2) / 4;        /* k = (b − a − 2)/4 ≥ 1 */

        /* compute triangular number t = k(k+1)/2 */
        seq_t t = k * (k + 1) / 2;

        if (best_t == -1 || t < best_t) best_t = t;
    }

    return best_t;   /* returns -1 if no suitable t was found */
}
