/*
 * A282416  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 469", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^((n+1)/2)-1)/9 (if n odd) a(n) = 10^(n/2) *
 *            (10^((n/2)+1)-1)/9 (if n even)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    idx_t zeros = n >> 1;          /* n/2 */
    if (n & 1) {                  /* odd n */
        idx_t k = zeros + 1;      /* (n+1)/2 */
        seq_t p = pow10(k);
        return (p - 1) / 9;
    } else {                      /* even n */
        idx_t ones = zeros + 1;   /* (n/2)+1 */
        seq_t ones_part = (pow10(ones) - 1) / 9;
        seq_t zeros_part = pow10(zeros);
        return zeros_part * ones_part;
    }
}
