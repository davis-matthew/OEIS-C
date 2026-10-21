/*
 * A267270  Binary representation of the n-th iteration of the "Rule 115"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = 1100 * 10^((3*n)/2 - 3) for even n > 0 a(n) =
 *            (10^(2*n + 1) - 9*10^((3*n - 1)/2) - 1)/9 for odd n using
 *            pow10(idx_t e) from the utils library
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if ((n & (idx_t)1) == 0) {              /* even n */
        idx_t exp = (3 * n) / 2 - 3;
        return pow10(exp) * (seq_t)1100;
    }

    /* odd n */
    idx_t exp1 = 2 * n + 1;
    idx_t exp2 = (3 * n - 1) / 2;

    seq_t term1 = pow10(exp1);
    seq_t term2 = pow10(exp2);

    seq_t tmp = term1 - term2 * (seq_t)9 - (seq_t)1;
    return tmp / (seq_t)9;
}
