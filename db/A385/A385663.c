/*
 * A385663  Binary numbers with an even number of 1s.
 * https://oeis.org/A385663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the decimal integer whose digits are the binary
 *            representation of the n‑th binary number with an even number of
 *            1s
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 512, u64 n <= 9128,
 *            u128 n <= 9128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t m = 0; ; ++m) {
        /* Count 1‑bits of m */
        unsigned pc = 0;
        idx_t t = m;
        while (t) {
            pc++;
            t &= t - 1;
        }
        if (pc & 1)      /* odd number of 1s – skip */
            continue;

        ++found;
        if (found == n) {
            if (m == 0)          /* first term */
                return (seq_t)0;

            /* Determine bit length of m */
            idx_t bitlen = 0;
            t = m;
            while (t) {
                bitlen++;
                t >>= 1;
            }

            /* Convert binary digits of m into a decimal integer */
            seq_t res = 0;
            for (idx_t i = 0; i < bitlen; ++i) {
                idx_t shift = bitlen - 1 - i;
                idx_t bit   = (m >> shift) & 1;
                res = res * 10 + (seq_t)bit;
            }
            return res;
        }
    }
}
