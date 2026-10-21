/*
 * A284634  Numbers with digits 4 and 6 only.
 * https://oeis.org/A284634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 * A032810(n)
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the number of digits k:
       k = floor(log2(n + 1))  (with k ≥ 1 for n ≥ 1) */
    seq_t x = (seq_t)n + 1;     /* x = n + 1 */
    idx_t k = 0;
    seq_t power = 1;            /* power = 2^k */
    while (power * 2 <= x) {
        power <<= 1;
        ++k;
    }

    /* m is the offset within the group of length k numbers (0‑based) */
    seq_t m = (seq_t)n - power + 1;

    /* Construct the decimal number by mapping binary bits to digits 4/6 */
    seq_t result = 0;
    for (idx_t i = 0; i < k; ++i) {
        seq_t bit = (m >> (k - 1 - i)) & 1;
        result = result * 10 + (bit ? 6 : 4);
    }
    return result;
}
