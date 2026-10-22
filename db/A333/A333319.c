/*
 * A333319  a(n) is the number of subsets of {1..n} that contain exactly 3 odd
 *          and 1 even numbers.
 * https://oeis.org/A333319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(ceil(n/2),3) * binomial(floor(n/2),1)
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 802, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom3(idx_t n)
{
    if (n < 3) return (seq_t)0;
    /* n*(n-1)*(n-2)/6 fits in seq_t for n <= 2^32 */
    return (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2) / (seq_t)6;
}

seq_t a(idx_t n)
{
    idx_t floor_half = n / 2;          /* floor(n/2) */
    idx_t ceil_half  = (n + 1) / 2;    /* ceil(n/2)  */
    if (floor_half == 0) return (seq_t)0;
    return (seq_t)floor_half * binom3(ceil_half);
}
