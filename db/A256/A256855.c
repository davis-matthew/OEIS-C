/*
 * A256855  Number of ordered ways to write n as x*(3*x-1)/2 + y*(3*y+1)/2 +
 *          z*(3*z+1), where x and y are nonnegative integers and z is an
 *          integer.
 * https://oeis.org/A256855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (x,y,z) with * n = x*(3*x-1)/2
 *            + y*(3*y+1)/2 + z*(3*z+1), * x,y >= 0, z integer
 * Range:     u8 n <= 5, u16 n <= 1016, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t x = 0; ; ++x) {
        seq_t xf = ( (seq_t)3 * x * x - x ) / 2;   /* f(x) = x*(3*x-1)/2 */
        if (xf > n) break;

        for (idx_t y = 0; ; ++y) {
            seq_t gy = ( (seq_t)3 * y * y + y ) / 2;   /* g(y) = y*(3*y+1)/2 */
            seq_t sum = xf + gy;
            if (sum > n) break;

            seq_t rem = n - sum;                      /* remainder to be expressed by z */
            seq_t D   = (seq_t)12 * rem + 1;          /* D = 1 + 12*rem */

            seq_t s = floor_sqrt(D);
            if (s * s == D) {
                /* Check if z = (-1 + s)/6 is integer */
                if ( (s - 1) % 6 == 0 )
                    ++total;
                /* Check if z = (-1 - s)/6 is integer */
                if ( (s + 1) % 6 == 0 )
                    ++total;
            }
        }
    }

    return total;
}
