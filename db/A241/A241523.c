/*
 * A241523  The number of P-positions in the game of Nim with up to 5 piles,
 *          allowing for piles of zero, such that the number of objects in
 *          each pile does not exceed n.
 * https://oeis.org/A241523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(4*b) + 10*2^(2*b)*c^2 + 5*c^4 * where b =
 *            floor(log2(n)) for n>0 * and c = n + 1 - 2^b * For n = 0, a(0) =
 *            1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute b = floor(log2(n)) */
    idx_t b = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++b;
    }

    /* 2^b as seq_t */
    seq_t pow2b = ((seq_t)1 << b);

    /* c = n + 1 - 2^b */
    seq_t c = ((seq_t)(n + 1)) - pow2b;

    /* c^2 and c^4 */
    seq_t c2 = c * c;
    seq_t c4 = c2 * c2; /* c^4 */

    /* 2^(4*b) */
    seq_t term1 = ((seq_t)1 << (4 * b));

    /* 2^(2*b) */
    seq_t pow2_2b = ((seq_t)1 << (2 * b));

    /* term2 = 10 * 2^(2*b) * c^2 */
    seq_t term2 = pow2_2b * c2;
    term2 *= (seq_t)10;

    /* term3 = 5 * c^4 */
    seq_t term3 = c4;
    term3 *= (seq_t)5;

    return term1 + term2 + term3;
}
