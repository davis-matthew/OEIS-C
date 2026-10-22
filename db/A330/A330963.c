/*
 * A330963  For any n >= 0: consider all pairs of numbers (x, y) whose binary
 *          representations can be interleaved (or shuffled) to produce the
 *          binary representation of n (possibly with leading zeros); a(n) is
 *          the least possible value of abs(x^2 - y^2).
 * https://oeis.org/A330963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min_{all partitions of the bits of n} |x^2 - y^2|
 * Range:     u8 n <= 15, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where x and y are the binary numbers obtained by distributing the
 *   bits of n (keeping their relative order) between the two numbers.
 *   The implementation enumerates all 2^k subsets of the k bits of n
 *   and keeps the smallest absolute difference of squares.  Complexity
 *   O(2^k * k), which is trivial for n up to about 10000 (k ≤ 14). */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) as defined for OEIS sequence A330963. */
seq_t a(idx_t n)
{
    /* Special case for n = 0; its binary representation is "0". */
    if (n == 0)
        return 0;

    /* Determine the number of bits needed to represent n.  This is the
     * floor(log2(n)) + 1 for n > 0.  We count bits manually to avoid
     * external helpers.  For n <= 10000 the result is <= 14. */
    idx_t bitlen = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        ++bitlen;
        tmp >>= 1;
    }

    /* Total number of masks (partitions) of the bit positions. */
    idx_t total_masks = (idx_t)1 << bitlen;

    seq_t best = 0;
    bool first = true;

    /* Enumerate every possible mask (0 … total_masks-1).  For each mask
     * the ith bit (from 0 to bitlen-1) tells whether the corresponding
     * bit of n goes to x (1) or to y (0). */
    for (idx_t mask = 0; mask < total_masks; ++mask) {
        seq_t x = 0;
        seq_t y = 0;

        /* Build x and y by scanning bits of n from most significant to
         * least significant. */
        for (idx_t i = 0; i < bitlen; ++i) {
            /* Extract the i‑th bit of n, counting from the most
             * significant bit. */
            int bit = (int)((n >> (bitlen - 1 - i)) & 1U);

            if (mask & ((idx_t)1 << i))
                x = (x << 1) | (seq_t)bit;
            else
                y = (y << 1) | (seq_t)bit;
        }

        /* Compute the absolute difference of squares using only
         * allowed operations (seq_t multiplication is modulo the type). */
        seq_t sqx = x * x;
        seq_t sqy = y * y;
        seq_t diff = (sqx > sqy) ? (sqx - sqy) : (sqy - sqx);

        if (first || diff < best) {
            best = diff;
            first = false;
        }
    }

    return best;
}
