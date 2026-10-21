/*
 * A290194  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 705", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(2) = 5; for n >= 3: a(n) = 2^(n+1) - 4 +
 *            (n mod 2) (equivalently, for n > 3: a(n) = 2*a(n-1)+2 if n even,
 *            a(n) = 2*a(n-1)+5 if n odd)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    /* n >= 3 */
    seq_t p = pow2(n + 1);   /* p = 2^(n+1) */
    p -= (seq_t)4;           /* subtract 4 */
    p += (seq_t)(n & 1);     /* add parity term */
    return p;
}
