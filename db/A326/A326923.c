/*
 * A326923  a(n) is the number of iterations needed to reach 1 or 9 starting
 *          at n and using the map k -> (k/2 if k is even, otherwise k +
 *          (largest triangular number < k)). Set a(n) = -1 if the trajectory
 *          never reaches 1 or 9.
 * https://oeis.org/A326923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1458, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326923: a(n) = number of iterations needed to reach 1 or 9
   using the map k -> (k/2 if k even, otherwise k + (largest triangular
   number < k)).  Returns (seq_t)-1 if the trajectory would exceed a
   predefined step limit (conservatively set to 100 000).  The formula is
   implemented directly from the definition, so no recurrence or
   hard‑coded table is used. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Return the largest triangular number T_m < k, where
   T_m = m*(m+1)/2.  Uses only the seq_t operations permitted. */
static seq_t largest_triangular_less_than(seq_t k)
{
    /* For k <= 1 there is no positive triangular number < k. */
    if (k <= 1) {
        return (seq_t)0;
    }

    /* Solve m = floor( (sqrt(8*k + 1) - 1) / 2 ). */
    seq_t eight_k = k * (seq_t)8;
    seq_t discr = eight_k + (seq_t)1;          /* discriminant 8*k+1      */
    seq_t sqrt_disc = floor_sqrt(discr);       /* floor(sqrt(discr))     */
    seq_t m = (sqrt_disc - (seq_t)1) / (seq_t)2;

    /* Compute T = m*(m+1)/2. */
    seq_t T = (m * (m + (seq_t)1)) / (seq_t)2;

    /* If k itself is triangular, step back one. */
    if (T >= k) {
        if (m == (seq_t)0) {
            return (seq_t)0;
        }
        m = m - (seq_t)1;
        T = (m * (m + (seq_t)1)) / (seq_t)2;
    }

    return T;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;           /* current value on trajectory   */
    seq_t steps = (seq_t)0;       /* number of applied steps       */
    const seq_t MAX_STEPS = (seq_t)100000;   /* safety limit */

    while (k != (seq_t)1 && k != (seq_t)9) {
        if (steps >= MAX_STEPS) {
            /* Indicates non‑termination within the cutoff. */
            return (seq_t)-1;
        }
        steps++;

        /* Even case: k -> k/2.  Use modular test to avoid
           depending on bitwise operators for arbitrary precision. */
        if (k % (seq_t)2 == (seq_t)0) {
            k = k / (seq_t)2;
        } else {
            seq_t tri = largest_triangular_less_than(k);
            k = k + tri;
        }
    }

    return steps;
}
