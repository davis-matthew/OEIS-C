/*
 * A241731  The number of P-positions in the game of Nim with up to 5 piles,
 *          allowing for piles of zero, such that the number of objects in the
 *          largest pile is n.
 * https://oeis.org/A241731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0; otherwise a(n) = 10*2^(2*b)*(2*c-1) + 20*c^3
 *            - 30*c^2 + 20*c - 5, where b = floor(log2(n)), c = n + 1 - 2^b
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* b = floor(log2(n)) */
    unsigned b = floor_log2_uint64(n);

    /* 2^b as seq_t */
    seq_t two_pow_b = pow2(b);

    /* c = n + 1 - 2^b */
    seq_t c = (seq_t)(n + 1) - two_pow_b;

    /* term1 = 10 * 2^(2*b) * (2*c - 1) */
    seq_t pow2_2b   = pow2((idx_t)(2 * b));
    seq_t two_c_minus_1 = (seq_t)2 * c - (seq_t)1;
    seq_t term1 = (seq_t)10 * pow2_2b * two_c_minus_1;

    /* term2 = 20*c^3 - 30*c^2 + 20*c - 5 */
    seq_t c2 = c * c;
    seq_t c3 = c2 * c;
    seq_t term2 = (seq_t)20 * c3 - (seq_t)30 * c2 + (seq_t)20 * c - (seq_t)5;

    return term1 + term2;
}
