/*
 * A282003  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 413", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = a(1) = 1 for even n≥2: a(n) = 110 * 10^(n‑2) for odd n≥1:
 *            a(n) = (10^n – 1) / 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }
    if ((n & 1) == 0) {                /* even n */
        return 110 * pow10(n - 2);
    } else {                            /* odd n */
        return (pow10(n) - 1) / 9;
    }
}
