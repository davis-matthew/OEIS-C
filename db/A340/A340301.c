/*
 * A340301  a(n) = n * floor(log_2(n)).
 * https://oeis.org/A340301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * floor(log_2(n))
 * Range:     u8 n <= 51, u16 n <= 5461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 for valid indices, but guard against 0 */
    if (n == 0)
        return 0;

    /* compute floor(log2(n)) */
    unsigned log2 = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++log2;
    }

    /* multiply in seq_t to avoid intermediate overflow */
    return (seq_t)n * (seq_t)log2;
}
