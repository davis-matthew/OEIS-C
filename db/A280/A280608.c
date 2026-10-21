/*
 * A280608  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 294", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A280608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280608
   a(0) = 1, a(1) = 3, a(2) = 4, a(3) = 14
   For n ≥ 4:
     if n is even:  a(n) = 1 + 2^n
     if n is odd:   a(n) = 1 + 7 * 2^(n-2)
*/
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)14;

    if ((n & 1ULL) == 0) {          /* n even (n ≥ 4) */
        return (seq_t)1 + pow2(n);   /* 1 + 2^n */
    } else {                         /* n odd (n ≥ 5) */
        seq_t t = pow2(n - 2);       /* 2^(n-2) */
        t = t * (seq_t)7;            /* 7 * 2^(n-2) */
        return t + (seq_t)1;         /* 1 + 7 * 2^(n-2) */
    }
}
