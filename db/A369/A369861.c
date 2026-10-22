/*
 * A369861  The orbit of n under iterations of x -> concatenate(A048762(x),
 *          A055400(x)) enters a pseudo-loop x(k) = a^3 *
 *          10^((k-k0)*A055642(b)) + b for k > k0. This sequence lists the
 *          b-value.
 * https://oeis.org/A369861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 17, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digit_len, floor_cube_root, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A369861
   Computes the stable cube excess b for the orbit of n under
   f(x) = concat( floor(cuberoot(x))^3 , x - floor(cuberoot(x))^3 ).
   The orbit eventually reaches a state where the excess does not change
   (b becomes constant).  That constant is returned.  The algorithm
   iterates directly, using the provided helpers:
     - floor_cube_root(seq_t)
     - pow10(idx_t)
     - digit_len(seq_t)
   It works for all underlying seq_t sizes (8‑128 bit integers or arbitrary
   precision), and has negligible runtime for n up to at least 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    /* x begins as the starting integer n */
    seq_t x = (seq_t)n;

    /* Iterate until the cube excess stabilises */
    while (true) {
        /* Current step: compute c, b */
        seq_t a0 = floor_cube_root(x);          /* floor(cuberoot(x)) */
        seq_t cube0 = a0 * a0 * a0;             /* a0^3       */
        seq_t b0   = x - cube0;                 /* excess     */
        idx_t L0   = digit_len(b0);             /* number of digits of b0   */
        seq_t powL = pow10((idx_t)L0);          /* 10^L0       */

        /* Next value of the orbit */
        seq_t next = cube0 * powL + b0;

        /* Compute b of the next value */
        seq_t a1   = floor_cube_root(next);
        seq_t cube1 = a1 * a1 * a1;
        seq_t b1   = next - cube1;

        /* If excess remains unchanged, we have reached the pseudo‑loop */
        if (b1 == b0)
            return b0;

        /* Prepare for next iteration */
        x = next;
    }
}
