/*
 * A285835  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 107", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2^(n/2) for n > 6 and even a(n) = 2^(n+1) - 1 for n
 *            > 6 and odd initial values: a(0)=1, a(1)=3, a(2)=6, a(3)=15,
 *            a(4)=12, a(5)=63, a(6)=24 (The sequence is known to satisfy the
 *            above for all n >= 0.)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    switch (n) {
        case 0: return 1;
        case 1: return 3;
        case 2: return 6;
        case 3: return 15;
        case 4: return 12;
        case 5: return 63;
        case 6: return 24;
        default:
            if ((n & 1) == 0) {            /* even n > 6 */
                seq_t p_n  = pow2(n);
                seq_t p_half = pow2(n >> 1);
                return p_n - p_half;
            } else {                        /* odd n > 6 */
                seq_t p_n1 = pow2(n + 1);
                return p_n1 - 1;
            }
    }
}
