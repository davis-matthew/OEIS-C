/*
 * A243589  Numbers returned when each binary digit of n is replaced by the
 *          sum modulo 2 of the digits to its (wrapped) left and (wrapped)
 *          right.
 * https://oeis.org/A243589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   a(n) = ror(n) XOR rol(n) where ror and rol are one‑bit circular
 *            rotations of n's binary representation
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    seq_t x = n;
    unsigned t = bitlen_seq_t(x);
    /* Build mask of t lowest bits */
    seq_t mask = 0;
    for (unsigned i = 0; i < t; ++i)
        mask |= ((seq_t)1 << i);

    seq_t ror = ((x >> 1) | (x << (t - 1))) & mask;   /* rotate right */
    seq_t rol = ((x << 1) | (x >> (t - 1))) & mask;   /* rotate left */

    return rol ^ ror;
}
