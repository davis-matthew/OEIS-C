/*
 * A331857  a(n) is the greatest value obtained by partitioning the binary
 *          representation of n into consecutive blocks, and then reversing
 *          those blocks.
 * https://oeis.org/A331857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max{ value of (reversed blocks concatenated) } over all
 *            ways of partitioning the binary representation of n into
 *            consecutive blocks. The value is interpreted in base‑2 (i.e.,
 *            concatenated reversed blocks as a binary number)
 * Range:     u8 n <= 255, u16 n <= 7999, u32 n <= 7999, u64 n <= 7999,
 *            u128 n <= 7999, big any n
 * Helpers:   reverse_bits_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/reverse_bits_u64.h"

/* Compute a(n).  All computations are done on
   idx_t (uint64_t) for bit extraction and on
   seq_t for the final big‑integer accumulation.  */
seq_t a(idx_t n)
{
    /* By definition a(0) = 0.  */
    if (n == 0)
        return (seq_t)0;

    /* Length of the binary representation of n.  */
    unsigned len = 0;
    idx_t tmp = n;
    while (tmp)
    {
        ++len;
        tmp >>= 1;
    }

    /* Array of bits of n: bits[0] is the most significant bit.  */
    unsigned char bits[65];        /* len <= 64 for uint64_t */
    for (unsigned i = 0; i < len; ++i)
        bits[i] = (n >> (len - 1 - i)) & 1;

    /* Dynamic programming array for suffixes (dp[0] is the answer). */
    seq_t dp[65];
    dp[len] = (seq_t)0;            /* empty suffix has value 0 */

    for (int i = (int)len - 1; i >= 0; --i)
    {
        seq_t best = (seq_t)0;
        idx_t block_val = 0;       /* accumulate bits of the current block */
        for (unsigned l = 1; l <= len - i; ++l)
        {
            block_val = (block_val << 1) | bits[i + l - 1];

            /* Reverse the block.  Result fits in idx_t (≤64 bits).  */
            idx_t rev = reverse_bits_u64(block_val, l);

            /* Shift the reversed block to its correct position in the
               final binary number and add the best suffix after it.  */
            unsigned shift = len - (i + l);
            seq_t candidate = ((seq_t)rev << shift) | dp[i + l];

            if (candidate > best)
                best = candidate;
        }
        dp[i] = best;
    }

    return dp[0];
}
