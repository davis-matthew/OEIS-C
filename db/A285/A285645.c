/*
 * A285645  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 65", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285645
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

/* A285645: Diagonal of Rule 65 von Neumann neighborhood
   Implementation based on the OEIS recurrence and explicit
   closed‑form for odd and even indices.

   For n = 0   → 1
   For n = 1   → 2
   For n = 2   → 1
   For n = 3   → 12
   For n = 4   → 1
   For n ≥ 5:
       if n is even → 3
       if n is odd  → 2 * (2^n – 2)
*/

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the initial special cases */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 1;
    if (n == 3) return 12;
    if (n == 4) return 1;

    /* For even n ≥ 6 the value is 3 */
    if ((n & 1ULL) == 0ULL)
        return 3;

    /* For odd n ≥ 5 the value is 2 * (2^n – 2) */
    seq_t val = pow2(n); /* 2^n */
    val -= 2;            /* 2^n – 2 */
    val *= 2;            /* 2 * (2^n – 2) */
    return val;
}
