/*
 * A284381  Numbers k with digits 5 and 8 only.
 * https://oeis.org/A284381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = map (n‑1) in binary to digits 5/8 (0→5, 1→8)
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1022, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t idx = n - 1;

    /* Find d = floor(log2(idx + 2)) */
    idx_t v = idx + 2;
    idx_t d = 0;
    idx_t tmp = v;
    while (tmp >>= 1)
        ++d;

    /* Compute 2^d */
    idx_t pw = 1;
    for (idx_t i = 0; i < d; ++i)
        pw <<= 1;

    /* Offset within the d‑digit 5/8 numbers */
    idx_t offset = idx - (pw - 2);

    seq_t result = 0;
    for (idx_t i = 0; i < d; ++i) {
        idx_t bit = (offset >> (d - 1 - i)) & 1;
        result = result * 10 + (bit ? (seq_t)8 : (seq_t)5);
    }
    return result;
}
