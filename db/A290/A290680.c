/*
 * A290680  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 929", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(n+1) - 1) / 9 for n > 3, with a(0)=1, a(1)=10,
 *            a(2)=111, a(3)=1110
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the few special cases explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)111;
    if (n == 3) return (seq_t)1110;

    /* for n >= 4 compute the repunit of length n+1: 111…1 */
    seq_t result = 0;
    idx_t len = n + 1;
    for (idx_t i = 0; i < len; ++i) {
        result = result * 10 + 1;   /* arithmetic is performed in seq_t */
    }
    return result;
}
