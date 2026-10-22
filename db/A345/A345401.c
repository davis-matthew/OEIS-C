/*
 * A345401  a(n) is the unique odd number h such that BCR(h*2^m-1) = 2n
 *          (except for BCR(0) = 1) where BCR is bit complement and reverse
 *          per A036044.
 * https://oeis.org/A345401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = BCR(2*n) + 1 for n >= 1; a(0) = 1 * BCR(x) =
 *            reverse(bitwise complement of x within its bit length)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

static inline seq_t reverse_bits(seq_t v, unsigned k) {
    seq_t rev = 0;
    unsigned i;
    for (i = 0; i < k; ++i) {
        if (v & ((seq_t)1 << i))
            rev |= ((seq_t)1 << (k - 1 - i));
    }
    return rev;
}

static inline seq_t bcr(seq_t x) {
    if (x == (seq_t)0)
        return (seq_t)1;
    unsigned k = bitlen_seq_t(x);
    seq_t mask = (((seq_t)1 << k) - 1);
    seq_t comp = (~x) & mask;
    return reverse_bits(comp, k);
}

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)1;
    seq_t x = (seq_t)2 * (seq_t)n;
    return bcr(x) + (seq_t)1;
}
