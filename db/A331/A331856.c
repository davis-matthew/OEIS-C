/*
 * A331856  a(n) is the least value obtained by partitioning the binary
 *          representation of n into consecutive blocks, and then reversing
 *          those blocks.
 * https://oeis.org/A331856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimum value obtained by partitioning the binary
 *            representation of n into consecutive blocks, reversing each
 *            block, and concatenating them
 * Range:     u8 n <= 509, u16 n <= 9568, u32 n <= 9568, u64 n <= 9568,
 *            u128 n <= 9568, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Determine the number of bits in n.  Use len = 1 for n == 0. */
    idx_t len;
    if (n == 0)
        len = 1;
    else {
        len = 0;
        idx_t tmp = n;
        do {
            ++len;
            tmp >>= 1;
        } while (tmp);
    }

    /* Store the individual bits in an array with bits[0] = most‑significant bit. */
    uint8_t bits[64];
    for (idx_t i = 0; i < len; ++i)
        bits[i] = (n >> (len - 1 - i)) & 1;

    /* Dynamic programming:
       dp[pos] = minimal value obtainable from bits[pos .. len-1]. */
    seq_t dp[65];
    dp[len] = 0;                /* base case: empty suffix has value 0 */

    for (idx_t pos = len; pos-- > 0;) {   /* iterate pos from len-1 down to 0 */
        seq_t best = (seq_t)n;            /* a(n) is never larger than n      */
        seq_t rev  = (seq_t)bits[pos];    /* reversed value of the current block */

        for (idx_t end = pos; end < len; ++end) {
            if (end > pos)                     /* grow the block by one bit   */
                rev |= ((seq_t)bits[end] << (end - pos));

            idx_t shift_bits = len - end - 1;   /* remaining bits after the block */
            seq_t val = (rev << shift_bits) | dp[end + 1];

            if (val < best)
                best = val;
        }
        dp[pos] = best;
    }

    return dp[0];
}
