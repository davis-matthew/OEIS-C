/*
 * A290683  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 929", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 1 for n > 3, a(0) = 1, a(1) = 1, a(2) = 7, a(3)
 *            = 7
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper pow2(idx_t) which returns 2^exp as seq_t. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2 || n == 3)
        return (seq_t)7;          /* a(2) and a(3) are both 7 */

    /* For n >= 4: a(n) = 2^(n+1) - 1 */
    seq_t val = pow2((idx_t)(n + 1)); /* compute 2^(n+1) */
    val -= (seq_t)1;                 /* subtract 1 */
    return val;
}
