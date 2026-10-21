/*
 * A279879  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 213", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A279879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n/2+1) - 1 for even n a(n) = 2^((n+1)/2) *
 *            (2^((n+1)/2)-1) for odd n a(0) = 1, a(1) = 2, a(2) = 2
 *            (corresponds to OEIS A279879)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 2;

    if ((n & 1) == 0) {                /* n even */
        seq_t p = pow2(n / 2 + 1);     /* 2^(n/2+1) */
        return p - 1;
    } else {                           /* n odd */
        seq_t p = pow2((n + 1) / 2);   /* 2^((n+1)/2) */
        return p * (p - 1);
    }
}
