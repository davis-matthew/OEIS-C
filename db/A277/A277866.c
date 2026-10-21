/*
 * A277866  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 3", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A277866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1-(-2)^n+2*(-1)^n+2^n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

/* Return the n-th term of OEIS A277866.
   The formula simplifies to:
     a(n) = 1                           if n is even,
     a(n) = 2^(n+1) - 3                 if n is odd.   */
seq_t a(idx_t n) {
    if ((n & 1u) == 0) {          /* n even */
        return (seq_t)1;
    } else {                      /* n odd */
        seq_t v = pow2(n + 1);    /* v = 2^(n+1) */
        v -= (seq_t)3;            /* v = 2^(n+1) - 3 */
        return v;
    }
}
