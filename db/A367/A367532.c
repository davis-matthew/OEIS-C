/*
 * A367532  The number of ways of tiling the n X n grid up to 90-degree
 *          rotation by a tile that is not fixed under 180-degree rotation.
 * https://oeis.org/A367532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n-1) = 256^(n^2 - n) a(2*n) = 4^(n^2 - 1) * (2 + 4^(n^2) +
 *            64^(n^2))
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Offset is 1; n >= 1 for all valid calls. */
    if (n & 1) { /* odd n: n = 2*m-1 */
        idx_t m   = (n + 1) / 2;          /* m = (n+1)/2 */
        idx_t exp = m * (m - 1);          /* exponent = m^2 - m */
        return pow_int((seq_t)256, exp);
    } else { /* even n: n = 2*m */
        idx_t m  = n / 2;                 /* m = n/2 */
        idx_t m2 = m * m;                 /* m^2 */

        seq_t factor1 = pow_int((seq_t)4, m2 - 1);
        seq_t term1   = pow_int((seq_t)4, m2);
        seq_t term2   = pow_int((seq_t)64, m2);

        seq_t sum = (seq_t)2 + term1 + term2;
        return factor1 * sum;
    }
}
