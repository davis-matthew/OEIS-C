/*
 * A290863  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 131", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 for n >= 1: if n is odd: a(n) = 2^(n+1) - 1 if n is
 *            even: let k = n/2 let m = 2^⌊log2(k)⌋ (largest power of 2 ≤ k)
 *            a(n) = 2^(n+1) - 2^(n-m+1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0u)          /* a(0) */
        return (seq_t)1;

    if (n & 1u) {         /* odd n */
        return pow2(n + 1) - (seq_t)1;
    }

    /* even n > 0 */
    idx_t k = n >> 1;     /* n / 2 */
    idx_t floor = 0;
    idx_t tmp = k;
    while (tmp >>= 1) ++floor;   /* floor = ⌊log2(k)⌋ */
    idx_t m = ((idx_t)1) << floor; /* largest power of 2 ≤ k */

    seq_t high = pow2(n + 1);
    seq_t low  = pow2(n - m + 1);
    return high - low;
}
