/*
 * A285777  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 89", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=1, a(3)=14; * a(n)=0 for n>3 even; *
 *            a(n)=2^(n+1)-1 for n>3 odd
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    if (n == 1u) return 2;
    if (n == 2u) return 1;
    if (n == 3u) return 14;
    if ((n & 1u) == 0u) return 0;
    return pow2(n+1) - 1;
}
