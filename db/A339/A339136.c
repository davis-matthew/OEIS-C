/*
 * A339136  Number of (undirected) cycles in the graph C_3 X P_n.
 * https://oeis.org/A339136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*3^(n-1) - 5*n - 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    seq_t term = pow3(n - 1);   /* 3^(n-1) */
    term = 9 * term;            /* 9 * 3^(n-1) */
    term -= 5 * (seq_t)n;       /* subtract 5*n */
    term -= 3;                  /* subtract 3 */
    return term;
}
