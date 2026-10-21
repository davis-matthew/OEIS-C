/*
 * A290682  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 929", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=7, a(3)=14. For n > 3, a(n) = 2^(n + 1) - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 3) {
        switch ((idx_t)n) {
            case 0u: return (seq_t)1;
            case 1u: return (seq_t)2;
            case 2u: return (seq_t)7;
            case 3u: return (seq_t)14;
            /* no other cases possible */
        }
    }
    seq_t p = pow2(n + 1);
    return p - 1;
}
