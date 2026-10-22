/*
 * A351308  Sum of the cubes of the square divisors of n.
 * https://oeis.org/A351308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d^2|n} (d^2)^3
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 1680, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t k2 = k * k;            /* k^2 */
        if (k2 > n)
            break;
        if (n % k2 == 0) {
            seq_t sk  = (seq_t)k;    /* k  */
            seq_t k2t = sk * sk;     /* k^2 */
            seq_t k4  = k2t * k2t;   /* k^4 */
            seq_t k6  = k4 * k2t;    /* k^6 */
            sum += k6;
        }
    }
    return sum;
}
