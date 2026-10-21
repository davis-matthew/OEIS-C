/*
 * A277929  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 5", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A277929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277929:
   a(0) = 1
   a(1) = 1
   a(n) = 0                    for n > 1 even
   a(n) = 2^(n+1) - 1          for n > 1 odd
   (equivalently a(n) = 2^n - 1 - (-2)^n for n > 1 odd) */

#include <stdint.h>                 /* for uint64_t (idx_t) */
#include "../utils/pow2.h"          /* for seq_t pow2(idx_t) */

seq_t a(idx_t n)
{
    /* handle initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* even indices > 1 return 0 */
    if ((n & 1) == 0) return (seq_t)0;

    /* odd indices > 1: 2^(n+1) - 1 */
    seq_t val = pow2(n + 1);
    return val - (seq_t)1;
}
