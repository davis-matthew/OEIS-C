/*
 * A387616  Numbers k that are less than distance j from j! for some positive
 *          integer j.
 * https://oeis.org/A387616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = j! - j + 1 + n - (j-1)^2, where j = ceil(sqrt(n+1))
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   factorial, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* For n >= 1.  The argument n is 1‑based. */
    idx_t np1 = n + 1;
    idx_t s = int_sqrt(np1);                 /* floor(sqrt(n+1)) */
    idx_t j = (s * s < np1) ? s + 1 : s;      /* ceil(sqrt(n+1)) */

    seq_t result = factorial(j);              /* j!  (seq_t)           */
    result = result - (seq_t)j;              /* j! - j                */
    result = result + 1;                     /* +1                     */
    result = result + (seq_t)n;              /* + n                    */

    idx_t jm1 = j - 1;
    idx_t jm1sq = jm1 * jm1;                  /* (j-1)^2                */
    result = result - (seq_t)jm1sq;           /* - (j-1)^2              */

    return result;
}
