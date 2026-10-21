/*
 * A289761  Maximum length of a perfect Wichmann ruler with n segments.
 * https://oeis.org/A289761
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( n^2 - (mod(n,6)-3)^2 ) / 3 + n
 * Range:     u8 n <= 26, u16 n <= 441, u32 n <= 8634, u64 n <= 8634,
 *            u128 n <= 8634, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the formula directly, using a wider type for intermediates
       to avoid overflow on small seq_t implementations. */
    uint64_t nn = (uint64_t)n;            /* n as unsigned 64-bit */
    int64_t m   = (int64_t)(n % 6);       /* 0 .. 5 as signed 64-bit */
    int64_t diff = m - 3;                 /* -3 .. 2 */
    uint64_t diff_sq = (uint64_t)(diff * diff); /* squared, 0 .. 9 */

    uint64_t num = nn * nn;                /* n^2 */
    uint64_t val = (num - diff_sq) / 3 + nn;

    return (seq_t)val;
}
