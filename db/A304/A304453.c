/*
 * A304453  An expanded binary notation for n: the normal binary expansion for
 *          n is expanded by mapping each 1 to 10 and retaining the existing
 *          0's.
 * https://oeis.org/A304453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal integer whose decimal digits are the binary *
 *            expansion of n with each binary 1 replaced by the digit sequence
 *            10. * The sequence is defined for n ≥ 0 with a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 46, u64 n <= 1534,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset case: a(0) = 0. */
    if (n == 0)
        return 0;

    /* Find the most significant set bit index (0‑based). */
    idx_t msb = 0;
    for (idx_t tmp = n >> 1; tmp; tmp >>= 1)
        msb++;

    /* Build the decimal number by processing bits from msb to lsb. */
    seq_t res = 0;
    for (idx_t i = msb; ; --i) {
        /* Extract the i‑th bit of n. */
        idx_t bit = (n >> i) & 1ULL;

        if (bit == 0) {
            /* Append '0' digit. */
            res = res * 10;
        } else {
            /* Append the digit sequence '10'. */
            res = res * 100 + 10;
        }

        if (i == 0)
            break;
    }

    return res;
}
