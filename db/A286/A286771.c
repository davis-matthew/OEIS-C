/*
 * A286771  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 221", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A286771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0,1; 0 for n = 2; a(n) = 10^n if n >= 3 even;
 *            a(n) = (10^n-1)/9 if n >= 3 odd
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 1;
    if (n == 2) return 0;

    seq_t ten_pow = pow10(n);
    if ((n & 1) == 0)                /* even */
        return ten_pow;
    else                              /* odd  */
        return (ten_pow - 1) / 9;
}
