/*
 * A285542  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 35", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1 - (-2)^n + 2^n + 2^(n/2)*(1+(-1)^n))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Even n: a(n) = 2^(n/2 + 1) - 1
       Odd  n: a(n) = 2^(n + 1)   - 1 */
    idx_t exp = (n & 1ULL) ? n + 1 : (n >> 1) + 1;
    seq_t val = pow2(exp);
    return val - 1;
}
