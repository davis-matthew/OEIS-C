/*
 * A375184  In the binary expansion of n, complement bits at even positions
 *          and to the right of a 0 (the most significant bit corresponding to
 *          position 1).
 * https://oeis.org/A375184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute sequence A375184.
 * In the binary expansion of n, a bit at an even position is
 * complemented iff the immediately preceding odd-position bit is 0.
 *
 * This implements the definition from the OEIS entry.  The function
 * works for any unsigned integer type seq_t and idx_t (uint64_t). */
seq_t a(idx_t n)
{
    seq_t res = 0;
    int bitlen = 0;
    idx_t tmp = n;

    /* Determine the number of bits in n (0 → bitlen = 0). */
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }

    /* Process bits from most significant to least significant. */
    int orig_prev = -1;            /* previous bit (original, not flipped) */
    for (int pos = 1; pos <= bitlen; ++pos) {
        int shift = bitlen - pos;
        int orig_bit = (int)((n >> shift) & 1ULL);
        int res_bit  = orig_bit;

        if ((pos & 1) == 0 && orig_prev == 0) {
            /* Even position whose left neighbour was 0: flip. */
            res_bit ^= 1;
        }

        res = (res << 1) | (seq_t)res_bit;
        orig_prev = orig_bit;      /* store original bit for next step */
    }
    return res;
}
