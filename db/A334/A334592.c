/*
 * A334592  Number of zeros in XOR-triangle with first row generated from the
 *          binary expansion of n.
 * https://oeis.org/A334592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of zeros in the XOR-triangle generated from the
 *            binary expansion of n. The triangle is built by XORing adjacent
 *            entries until a single entry remains. All entries, including the
 *            initial row, are counted
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Maximum bits needed for idx_t (uint64_t). 64 is safe. */
#define MAX_BITS 64

seq_t a(idx_t n)
{
    /* n must be >= 1 for the sequence definition. */
    if (n == 0)
        return 0;

    /* Determine the number of significant bits of n. */
    idx_t len = 0;
    idx_t temp = n;
    do {
        len++;
        temp >>= 1;
    } while (temp > 0);

    /* Store the bits of n.  bits[0] is the most‑significant bit. */
    uint8_t bits[MAX_BITS];
    for (idx_t i = 0; i < len; ++i) {
        bits[i] = (n >> (len - 1 - i)) & 1;
    }

    seq_t zeros = 0;
    idx_t cur_len = len;

    /* Build the triangle row by row, counting zeros. */
    while (cur_len > 0) {
        for (idx_t i = 0; i < cur_len; ++i)
            if (bits[i] == 0)
                ++zeros;

        /* Compute the next row by XORing adjacent entries. */
        for (idx_t i = 0; i + 1 < cur_len; ++i)
            bits[i] = bits[i] ^ bits[i + 1];

        --cur_len;
    }

    return zeros;
}
