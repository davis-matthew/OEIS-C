/*
 * A350361  2-tone chromatic number of a tree with maximum degree n.
 * https://oeis.org/A350361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((5 + sqrt(1 + 8*n))/2)
 * Range:     u8 n <= 7, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the offset; use the formula directly */
    seq_t X = (seq_t)1 + (seq_t)8 * (seq_t)n;          /* 1 + 8*n   */
    seq_t s = floor_sqrt(X);                          /* floor(sqrt(1+8*n)) */

    seq_t sum   = (seq_t)5 + s;                       /* 5 + s     */
    seq_t base  = sum >> 1;                            /* floor((5+s)/2)   */
    seq_t rem   = sum & 1;                             /* (5+s) mod 2      */

    /* If sqrt(1+8*n) is exact, use ceil((5+s)/2); otherwise add 1
       because ceil((5+exact)/2) = floor((5+s)/2) + (5+s mod 2),
       and ceil((5+inexact)/2) = floor((5+s)/2) + 1. */
    if (s * s == X)                                     /* exact sqrt ? */
        return base + rem;                               /* ceil for integer sqrt */
    else
        return base + 1;                                 /* ceil for non‑integer sqrt */
}
