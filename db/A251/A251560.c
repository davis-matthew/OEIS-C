/*
 * A251560  Erroneous version of A064614.
 * https://oeis.org/A251560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n, * except if n is a power of 2, a(n) = 3^k where n =
 *            2^k; * if n is a power of 3, a(n) = 2^k where n = 3^k
 * Range:     i8 n <= 11, i16 n <= 11, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle powers of 2 */
    if ((n & (n - 1)) == 0) {
        idx_t k = 0;
        idx_t t = n;
        /* Count exponent k such that n == 2^k (or 1 for k==0) */
        while (t > 1) {
            t >>= 1;      /* divide by 2 */
            ++k;
        }
        return big_pow((seq_t)3, (seq_t)k);
    }

    /* Handle powers of 3 */
    idx_t k = 0;
    idx_t t = n;
    while (t % 3 == 0) {
        t /= 3;
        ++k;
    }
    if (t == 1) {           /* n == 3^k */
        return big_pow((seq_t)2, (seq_t)k);
    }

    /* Default: unchanged */
    return (seq_t)n;
}
