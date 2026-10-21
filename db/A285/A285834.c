/*
 * A285834  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 107", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) – 1 for odd n; a(0) = 1, a(2) = 3, a(6) = 12; for
 *            even n ≥ 4 and n ≠ 6: a(n) = 2^(n/2 + 1) – 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0)                     return 1;
    if (n == 2)                     return 3;
    if (n == 6)                     return 12;

    if (n & 1) {                    /* n is odd */
        seq_t pow = pow2(n + 1);
        return pow - 1;
    } else {                        /* n is even (>=4, n≠6) */
        seq_t pow = pow2(n / 2 + 1);
        return pow - 2;
    }
}
