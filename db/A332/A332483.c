/*
 * A332483  Numbers k such that sin(k) < 0 and cos(k) < 0.
 * https://oeis.org/A332483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer k such that sin(k) < 0 &&
 *            cos(k) < 0
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    while (count < n) {
        long double sk = sinl((long double)k);
        long double ck = cosl((long double)k);
        if (sk < 0.0L && ck < 0.0L) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
    return 0; /* never reached */
}
