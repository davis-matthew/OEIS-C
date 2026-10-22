/*
 * A367307  Reverse bits in blocks in binary expansion of n where blocks are
 *          separated by every second 1 bit starting from the most significant
 *          1 bit as the first separator.
 * https://oeis.org/A367307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse bits in blocks of binary expansion of n, blocks
 *            separated by every second 1-bit starting from the most
 *            significant 1-bit
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Number of bits needed to represent n (n>0). */
static inline idx_t bitlen_u64(idx_t n)
{
    idx_t len = 0;
    while (n) {
        n >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t bitlen = bitlen_u64(n);

    seq_t  result   = 0;   /* Accumulated answer. */
    seq_t  block    = 0;   /* Bits of the current block. */
    idx_t  block_len = 0;  /* Length of the current block. */
    idx_t  ones_seen = 0;  /* Count of 1s processed so far. */

    /* Process bits from most significant to least significant. */
    for (idx_t i = bitlen; i-- > 0;) {
        idx_t bit = (n >> i) & 1U;

        if (bit) {
            ++ones_seen;
            bool separator = (ones_seen & 1U) != 0U;  /* odd‑indexed 1 is a separator */

            if (separator) {
                /* Output reversed block (if any). */
                if (block_len) {
                    seq_t   rev   = 0;
                    seq_t   tmp   = block;

                    for (idx_t j = 0; j < block_len; ++j) {
                        rev = (rev << 1) | (tmp & (seq_t)1U);
                        tmp >>= 1;
                    }
                    result = (result << block_len) | rev;
                    block  = 0;
                    block_len = 0;
                }

                /* Output the separator 1. */
                result = (result << 1) | (seq_t)1U;
            } else {
                /* Non‑separator 1 – part of the current block. */
                block = (block << 1) | (seq_t)1U;
                ++block_len;
            }
        } else { /* bit == 0 – part of the current block. */
            block = (block << 1);
            ++block_len;
        }
    }

    /* Output the final block (after the last separator). */
    if (block_len) {
        seq_t rev = 0;
        seq_t tmp = block;

        for (idx_t j = 0; j < block_len; ++j) {
            rev = (rev << 1) | (tmp & (seq_t)1U);
            tmp >>= 1;
        }
        result = (result << block_len) | rev;
    }

    return result;
}
