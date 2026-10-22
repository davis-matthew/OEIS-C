/*
 * A308621  Number of ways to write n as a*(a+1)/2 + b*(b+1)/2 + 2^c*5^(3d)
 *          with a,b,c,d nonnegative integers.
 * https://oeis.org/A308621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered non‑negative integer solutions to n =
 *            a*(a+1)/2 + b*(b+1)/2 + 2^c * 5^(3d) (with a ≤ b)
 * Range:     u8 n <= 8, u16 n <= 518, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The count is obtained by exhaustive search over all feasible
   values of c, d, and the triangular parts a and b.   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Triangular number t(x) = x*(x+1)/2, avoiding a temporary overflow
   by halving one factor first. */
static inline seq_t triangular(idx_t x)
{
    seq_t ux = (seq_t)x;
    if (x & 1U) {
        return ((ux + 1) / 2) * ux;
    } else {
        return (ux / 2) * (ux + 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    seq_t target = (seq_t)n;
    seq_t count   = 0;

    seq_t pow125 = 1;   /* 5^(3d) */
    for (int d = 0; ; ++d) {
        seq_t pow2 = 1;   /* 2^c */
        for (int c = 0; ; ++c) {
            seq_t term = pow2 * pow125;
            if (term > target) break;   /* larger c only increases term */

            seq_t rem = target - term;   /* remaining to be split into two triangles */

            for (idx_t a_val = 0; ; ++a_val) {
                seq_t t_a = triangular(a_val);
                if (t_a > rem) break;

                seq_t rem2 = rem - t_a;

                /* Check whether rem2 is triangular.
                   8*rem2 + 1 must be a perfect square and odd. */
                seq_t s = rem2;
                s <<= 3;          /* s = 8*rem2 */
                s += 1;           /* s = 8*rem2 + 1 */

                seq_t sqrt_s = floor_sqrt(s);
                if (sqrt_s * sqrt_s == s && (sqrt_s & 1U) == 1U) {
                    /* Derive the corresponding b from the square root. */
                    seq_t b_val = (sqrt_s - 1) >> 1;
                    if (b_val >= a_val) {
                        ++count;   /* count each unordered pair exactly once */
                    }
                }
            }

            if (pow2 > target / 2) break;   /* avoid overflow on next shift */
            pow2 <<= 1;
        }

        if (pow125 > target / 125) break;  /* avoid overflow on next multiplication */
        pow125 *= 125;
    }

    return count;
}
