/*
 * A260309  Expansion of phi(q) * phi(-q^6) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A260309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{i,j∈ℤ, i² + 6j² = n} (-1)^j Count the representations
 *            of n as i²+6j² with weight (-1)^j, weighing each pair (i,j)
 *            individually. The implementation iterates over the admissible j,
 *            checks if n−6j² is a perfect square, and adds the appropriate
 *            contribution
 * Range:     i8 n <= 63, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 98 terms through n = 975 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Upper bound for j: 6*j*j ≤ n  ⇔  j ≤ floor(sqrt(n/6)) */
    idx_t j_max = (idx_t)floor_sqrt((seq_t)(n / 6));

    for (idx_t j = 0; j <= j_max; ++j) {
        /* t = n - 6*j² */
        seq_t t = (seq_t)n - (seq_t)6 * (seq_t)j * (seq_t)j;
        if (t < 0) continue;                 /* safety, should never happen */

        /* check whether t is a perfect square */
        seq_t sqrt_t = floor_sqrt(t);
        if (sqrt_t * sqrt_t != t) continue;

        /* number of i solutions: 1 if t==0, else 2 */
        seq_t num_i = (t == 0 ? (seq_t)1 : (seq_t)2);

        /* sign factor (-1)^j */
        seq_t sign = (j & 1) ? (seq_t)-1 : (seq_t)1;

        if (j == 0) {
            result += sign * num_i;
        } else {
            /* each positive j has a symmetric negative counterpart */
            result += (seq_t)2 * sign * num_i;
        }
    }

    return result;
}
