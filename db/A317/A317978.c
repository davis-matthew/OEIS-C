/*
 * A317978  The number of ways to paint the cells of the six convex regular
 *          4-polytopes using exactly n colors where n is the number of cells
 *          of each 4-polytope.
 * https://oeis.org/A317978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = 2*A063924(n)! / A273509(n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063924(n) gives the number of cells of the n‑th convex regular 4‑polytope.
   A273509(n) is twice the order of its rotational symmetry group. */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Constants: number of cells for the six regular 4‑polytopes
       and the corresponding values of A273509 (twice the rotational group order). */
    const idx_t cells[6]   = {5, 8, 16, 24, 120, 600};
    const idx_t group2[6]  = {120, 384, 384, 1152, 14400, 14400};

    if (n < 1 || n > 6)          /* sequence defined only for 1..6 */
        return 0;

    /* Compute 2 * (cells[n-1])!  and divide exactly by group2[n-1] */
    seq_t num    = factorial(cells[n-1]);   /* cells[n-1]!    */
    num          = num * 2;                 /* 2 * cells!    */
    seq_t denom  = (seq_t)group2[n-1];      /* denom is small enough for cast */
    return num / denom;
}
