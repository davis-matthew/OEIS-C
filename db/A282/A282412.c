/*
 * A282412  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 467", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (-1)^n)*(10^(1+n)-1)/18 for n > 1 Initial terms:
 *            a(0)=1, a(1)=10, a(2)=111, a(3)=0, a(4)=11111, …
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
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
    if (n == 1)
        return (seq_t)10;
    if (n & 1)
        return (seq_t)0;            /* odd n > 1 → 0 */
    /* even n ≥ 2: (10^(1+n)-1)/9 = decimal number with (n+1) ones */
    seq_t ten_pow = pow10(n + 1);
    return (ten_pow - (seq_t)1) / (seq_t)9;
}
