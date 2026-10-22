/*
 * A330299  a(n) is the number of subsets of {1..n} that contain exactly 1 odd
 *          and 3 even numbers.
 * https://oeis.org/A330299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/2) * binomial(floor(n/2), 3)
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 803, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t even = n / 2;              /* floor(n/2)  */
    idx_t odd  = n - even;           /* ceil(n/2)   */
    /* If there are fewer than 3 even numbers, binomial(even,3) will be 0 */
    return (seq_t)odd * binomial(even, 3);
}
