/*
 * A338696  Number of ways to write n as x^3 + y^2 + z*(3*z+2), where x and y
 *          are nonnegative integers, and z is an integer.
 * https://oeis.org/A338696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) with x,y ≥ 0 integers, z any
 *            integer, such that n = x^3 + y^2 + z*(3*z+2)
 * Range:     u8 n <= 4, u16 n <= 995, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out‑of‑range guard – offset is 1 */
    seq_t count = (seq_t)0;
    seq_t n_t   = (seq_t)n;

    for (idx_t x = 0; ; ++x) {
        seq_t x_t   = (seq_t)x;
        seq_t cube  = x_t * x_t * x_t;
        if (cube > n_t) break;            /* further x will only increase cube */

        seq_t remmax = n_t - cube;
        seq_t y_max_t = floor_sqrt(remmax);   /* maximum y satisfying y^2 ≤ remmax */

        for (idx_t y = 0; y <= (idx_t)y_max_t; ++y) {
            seq_t y_t   = (seq_t)y;
            seq_t sum_xy = cube + y_t * y_t;
            seq_t rem    = n_t - sum_xy;      /* remaining part to be z*(3*z+2) */

            /* Solve 3*z^2 + 2*z - rem = 0  →  D = 4 + 12*rem  */
            seq_t D      = (seq_t)4 + (seq_t)12 * rem;
            seq_t sqrtD  = floor_sqrt(D);

            if (sqrtD * sqrtD != D) continue;      /* D not a perfect square */

            seq_t test1 = sqrtD - (seq_t)2;         /* numerator of (+) root */
            if (test1 % (seq_t)6 == (seq_t)0) {
                ++count;
                continue;
            }

            seq_t test2 = sqrtD + (seq_t)2;         /* numerator of (−) root */
            if (test2 % (seq_t)6 == (seq_t)0) {
                ++count;
            }
        }
    }
    return count;
}
