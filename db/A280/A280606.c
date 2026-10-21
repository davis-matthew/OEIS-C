/*
 * A280606  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 294", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A280606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, a(2)=100, a(3)=1110; For n ≥ 4: if n is even →
 *            a(n) = 1 + 10^n; if n is odd → a(n) = 1 + 111·10^(n–2); /* see
 *            OEIS A280606
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)100;
    if (n == 3) return (seq_t)1110;

    if ((n & 1) == 0) {
        /* even n */
        seq_t v = pow10(n);
        return v + (seq_t)1;
    } else {
        /* odd n (n ≥ 5) */
        seq_t v = pow10(n - 2);
        v *= (seq_t)111;
        return v + (seq_t)1;
    }
}
