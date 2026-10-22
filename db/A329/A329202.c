/*
 * A329202  a(n) = floor(2*log_2(n)) = floor(log_2(n^2)).
 * https://oeis.org/A329202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*log2(n)) = floor(log2(n^2))
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* compute n^2 in seq_t and take floor(log2(n^2)) */
    seq_t prod = (seq_t)n * (seq_t)n;
    /* floor(log2(prod)) = bit length of prod - 1 */
    return (seq_t)(bitlen_seq_t(prod) - 1);
}
