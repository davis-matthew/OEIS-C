/*
 * A303850  Duplicate of A303848.
 * https://oeis.org/A303850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   if n is even: a(n) = n/2 + 9; if n is odd: let k = (n+1)/2; let
 *            d = number of decimal digits of k; then a(n) = k + 10^d - 10
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned int digits_of_idx(idx_t x) {
    unsigned int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n) {
    if ((n & 1ULL) == 0) {               /* even n */
        return (seq_t)(n / 2) + (seq_t)9;
    } else {                             /* odd n */
        idx_t k = (n + 1) / 2;
        unsigned int d = digits_of_idx(k);
        seq_t pow10 = 1;
        for (unsigned int i = 0; i < d; ++i) {
            pow10 *= (seq_t)10;
        }
        seq_t offset = pow10 - (seq_t)10;
        return (seq_t)k + offset;
    }
}
