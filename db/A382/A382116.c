/*
 * A382116  a(n) = floor(n*g+(g-1)/2), where g is the golden ratio.
 * https://oeis.org/A382116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*φ + (φ-1)/2) (φ = (1+√5)/2)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * The sequence can be computed exactly using only integer arithmetic:
 * a(n) = ((2n + floor_sqrt(5*(2n+1)^2)) - 1) / 4
 *
 * This follows from writing φ = (1+√5)/2 and simplifying floor
 * n·φ + (φ-1)/2 to an explicit integer expression involving
 * floor_sqrt(5*(2n+1)^2).  The expression is safe for all n ≥ 0
 * and works for arbitrary‑precision builds because only additions,
 * multiplications, subtractions, and a single integer square root are
 * used.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow in intermediate
       signed operations.  All arithmetic is done in seq_t. */
    seq_t nn = (seq_t)n;

    /* m = 2n + 1;  xx = 5 * m * m */
    seq_t m  = (seq_t)(2 * nn + 1);
    seq_t xx = (seq_t)5 * m * m;

    /* y = floor_sqrt(xx) = floor((2n+1) * √5) */
    seq_t y = floor_sqrt(xx);

    /* a(n) = (2n + y - 1) / 4  (integer division truncates downward) */
    seq_t res = (seq_t)(2 * nn + y - 1) / (seq_t)4;

    return res;
}
