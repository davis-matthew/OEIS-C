/*
 * A286865  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 814", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A286865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (for n < 4); otherwise a(n) = 2^(2^(floor_log2(n)-1)) -
 *            1 where floor_log2(x) is the integer part of log2(x)
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   big_pow, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Values for the first four indices are 1. */
    if (n < 4)
        return 1;

    /* Compute k = floor(log2(n)) - 1. */
    uint32_t fl = floor_log2_uint64(n);          /* returns an unsigned int */
    idx_t k = (idx_t)fl - 1;                     /* k >= 1 for n >= 4 */

    /* inner exponent = 2^k. */
    seq_t inner = ((seq_t)1) << k;               /* 2^k */

    /* 2^(2^k). */
    seq_t val = big_pow((seq_t)2, inner);

    /* Subtract 1 to get a(n). */
    return val - 1;
}
