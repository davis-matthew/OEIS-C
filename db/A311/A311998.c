/*
 * A311998  Coordination sequence Gal.3.22.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n >= 5 a(n) = 24 + floor((n-5)/6) * 28 + prefix[(n-5)%6]
 *            where prefix = [0,2,10,13,19,22] // partial sums of the pattern
 *            (pattern of successive differences from a(5) onwards is
 *            2,8,3,6,3,6, repeating). For n = 0..4 the values are 1,4,8,13,17
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* values for n = 0 .. 4 */
    static const seq_t first[5] = {1, 4, 8, 13, 17};

    if (n < 5)
        return first[(size_t)n];

    /* pattern parameters for n >= 5 */
    static const seq_t period_sum = 28;                     /* 2+8+3+6+3+6 */
    static const seq_t prefix[6]  = {0, 2, 10, 13, 19, 22};  /* cumulative sums */

    idx_t k = n - 5;
    idx_t q = k / 6;          /* number of full periods after a(5)  */
    idx_t r = k % 6;          /* position inside current period   */

    /* a(5) = 24 */
    seq_t result = (seq_t)24 + (seq_t)(q * period_sum) + prefix[r];
    return result;
}
