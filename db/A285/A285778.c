/*
 * A285778  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 89", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=4, a(3)=7; for n>3, a(n)=0 if n even,
 *            a(n)=2^(n+1)-1 if n odd
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 3) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)4;
            case 3: return (seq_t)7;
        }
    }
    if ((n & 1) == 0)
        return (seq_t)0;          /* even n > 3 */
    return pow2(n + 1) - (seq_t)1; /* odd n > 3 */
}
