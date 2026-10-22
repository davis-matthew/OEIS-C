/*
 * A324403  a(n) = Product_{i=1..n, j=1..n} (i^2 + j^2).
 * https://oeis.org/A324403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n, j=1..n} (i^2 + j^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i2 = (seq_t)i * (seq_t)i;
        for (idx_t j = 1; j <= n; ++j) {
            result *= i2 + (seq_t)j * (seq_t)j;
        }
    }
    return result;
}
