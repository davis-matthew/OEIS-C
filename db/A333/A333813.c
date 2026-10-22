/*
 * A333813  a(n) = 2^(1 + floor(n*log_2(3))) - (3^n + 1).
 * https://oeis.org/A333813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(1 + floor(n*log_2(3))) - (3^n + 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   bitlen_seq_t, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* compute 3^n */
    seq_t p3 = seq_pow( (seq_t)3, n );

    /* floor(log_2(3^n)) + 1 = bit length of 3^n */
    unsigned bits = bitlen_seq_t(p3);

    /* compute 2^bits = 2^(1 + floor(n*log_2(3))) */
    seq_t p2 = seq_pow( (seq_t)2, (idx_t)bits );

    /* a(n) = 2^bits - (3^n + 1) */
    return p2 - (p3 + (seq_t)1);
}
