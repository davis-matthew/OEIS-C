/*
 * A290414  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 785", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(3)=14, a(4)=25 For n>4 even: a(n) =
 *            2^(n/2+1) * (2^(n/2) - 1) For n>4 odd : a(n) = 2^(n+1) - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)5;
        case 3: return (seq_t)14;
        case 4: return (seq_t)25;
        default:
            if ((n & 1) == 0) {          /* even n > 4 */
                idx_t k = n >> 1;         /* k = n/2 */
                seq_t pwk  = pow2(k);     /* 2^k */
                seq_t pwk1 = pow2(k + 1); /* 2^(k+1) */
                return pwk1 * (pwk - (seq_t)1);
            } else {                     /* odd n > 4 */
                return pow2(n + 1) - (seq_t)1;
            }
    }
}
