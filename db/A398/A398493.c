/*
 * A398493  Number of increasing 3-term arithmetic progressions in an n^5
 *          hypercube.
 * https://oeis.org/A398493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * Even n: a(n) = 16 * ( (n/2)^10 - (n/2)^5 ) * Odd n: Let r =
 *            (n^2 + 1)/2. * a(n) = ( r^5 - n^5 ) / 2 * These formulas avoid
 *            intermediate overflow when seq_t is * 8,16,32,64 or 128 bits and
 *            are valid for all n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 128,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                /* even n */
        seq_t m = n / 2;               /* m = n/2   */
        /* compute m^5 */
        seq_t m5 = 1;
        for (int i = 0; i < 5; ++i) m5 *= m;
        /* compute m^10 as (m^5)^2 */
        seq_t m10 = m5 * m5;
        /* a(n) = 16 * (m10 - m5) */
        return 16 * (m10 - m5);
    } else {                           /* odd n */
        /* r = (n^2 + 1) / 2 */
        seq_t n2 = n * n;
        seq_t r = (n2 + 1) / 2;
        /* compute r^5 */
        seq_t r5 = 1;
        for (int i = 0; i < 5; ++i) r5 *= r;
        /* compute n^5 */
        seq_t n5 = 1;
        for (int i = 0; i < 5; ++i) n5 *= n;
        /* a(n) = (r5 - n5) / 2 */
        return (r5 - n5) / 2;
    }
}
