/*
 * A290528  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 833", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 5. For n > 2: if n is even: a(n) =
 *            2^(n/2) − 1 if n is odd: a(n) = 2^(n+1) − 1 Uses the provided
 *            pow2 utility to compute powers of two exactly
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    if ((n & 1) == 0) {          /* even n > 2 */
        seq_t res = pow2(n / 2);
        res -= (seq_t)1;
        return res;
    } else {                     /* odd n > 2 */
        seq_t res = pow2(n + 1);
        res -= (seq_t)1;
        return res;
    }
}
