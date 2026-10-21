/*
 * A278760  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 73", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4) for n > 7, with initial terms:
 *            a(0)=1, a(1)=0, a(2)=6, a(3)=5, a(4)=0, a(5)=63, a(6)=0,
 *            a(7)=255. For n ≥ 5 odd: a(n) = 2^(n+1) – 1; for n ≥ 4 even:
 *            a(n) = 0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)0;

    /* Even indices ≥ 4 give 0 */
    if ((n & (idx_t)1) == 0) return (seq_t)0;

    /* Odd n ≥ 5: 2^(n+1) – 1 */
    seq_t val = pow2(n + 1);
    return val - (seq_t)1;
}
