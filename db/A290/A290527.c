/*
 * A290527  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 833", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n/2+1)*(2^(n/2)-1) for even n>2, a(n) = 2^(n+1)-1 for
 *            odd n>2 (a(0)=1, a(1)=2, a(2)=5)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
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
    if (n == 2) return 5;

    if (n & 1U) {           /* odd n > 2 */
        return pow2(n + 1) - 1;
    } else {                /* even n > 2 */
        idx_t k = n / 2;
        seq_t t = pow2(k);          /* t = 2^k */
        return (t * (t - 1)) << 1;  /* 2 * t * (t-1) */
    }
}
