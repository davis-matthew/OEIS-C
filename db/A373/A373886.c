/*
 * A373886  a(n) is the least number whose binary expansion can be obtained by
 *          reversing one or more consecutive bits in the binary expansion of
 *          n.
 * https://oeis.org/A373886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = minimum of numbers obtained by reversing at least one
 *            consecutive block of bits in the binary representation of n.
 *            Implementation brute‑forces all possible bit blocks
 * Range:     u8 n <= 509, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"
#include <stdbool.h>

/* Reverse the lowest 'len' bits of 'v'.  'len' is between 1 and 64. */
static seq_t reverse_bits(seq_t v, unsigned len)
{
    seq_t rev = 0;
    for (unsigned k = 0; k < len; ++k) {
        if (v & ((seq_t)1 << k))
            rev |= (seq_t)1 << (len - 1 - k);
    }
    return rev;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    unsigned bitlen = bitlen_seq_t(x);
    if (bitlen == 0)  /* n == 0 */
        return 0;
    seq_t best = x;

    for (unsigned i = 0; i < bitlen; ++i) {
        seq_t mask = 0;                 /* mask of 'len' ones, built incrementally */
        for (unsigned len = 1; len <= bitlen - i; ++len) {
            mask = (mask << 1) | 1;      /* now mask has 'len' low bits set to 1 */

            seq_t segment = (x >> i) & mask;   /* extract the block */
            seq_t rev = reverse_bits(segment, len);
            seq_t new_val = (x & ~(mask << i)) | (rev << i);  /* reconstruct */

            if (new_val < best)
                best = new_val;
        }
    }
    return best;
}
