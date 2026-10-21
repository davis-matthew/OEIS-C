/*
 * A295933  Number of (not necessarily maximal) cliques in the n-Sierpinski
 *          gasket graph.
 * https://oeis.org/A295933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(3 + 7*3^(n - 1))/6 for n > 1 a(1) = 8 Implements the
 *            OEIS sequence A295933: Number of (not necessarily maximal)
 *            cliques in the n‑Sierpinski gasket graph
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;

    /* p = 3^(n-1) */
    seq_t p     = pow3(n - 1);
    seq_t p_div = p / 6;      /* 3^(n-1) / 6  */
    seq_t p_mod = p % 6;      /* 3^(n-1) mod 6 */

    /* Compute (35*p + 15)/6 while avoiding overflow:
       (35* (p/6)  +  (35*(p%6)+15)/6 )
    */
    seq_t part = 35 * p_div;                       /* 35*(p/6)     */
    seq_t rem  = (35 * p_mod + 15) / 6;             /* remainder part */

    return part + rem;
}
