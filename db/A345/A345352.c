/*
 * A345352  The binary expansion of a(n) is obtained by left-padding the
 *          binary expansion of n with 0's so that it has 2^k digits for some
 *          k > 0 as small as possible and then reversing the first half and
 *          the second half of this binary expansion.
 * https://oeis.org/A345352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by left‑padding the binary expansion of n * to
 *            a length that is a power of two, then reversing the first * and
 *            second halves of that expansion
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) = 0 */

    /* Compute number of bits of n (bitlen) */
    idx_t bitlen = 0;
    for (idx_t t = n; t; t >>= 1) ++bitlen;

    /* Find the smallest power of two ≥ bitlen.  len is the total
       number of bits after padding. */
    idx_t len = 1;
    while (len < bitlen) len <<= 1;

    /* Number of leading zeros added during padding. */
    idx_t shift = len - bitlen;

    /* Build the result bit by bit from most significant to least
       significant.  result <<= 1 then OR the next bit. */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        idx_t src;
        if (i < len / 2)
            src = len / 2 - 1 - i;               /* src index within first half */
        else
            src = len - 1 - (i - len / 2);       /* src index within second half */

        int bit;
        if (src < shift)
            bit = 0;                              /* padded zeros */
        else {
            idx_t pos = src - shift;                /* position relative to n */
            idx_t idx = bitlen - 1 - pos;           /* bit index in n (LSB=0) */
            bit = (n >> idx) & 1;
        }

        result = (result << 1) | (seq_t)bit;
    }
    return result;
}
