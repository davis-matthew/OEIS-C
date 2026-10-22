/*
 * A330476  a(n) = Sum_{m=2..n} floor(n/m)^2.
 * https://oeis.org/A330476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=2..n} floor(n/m)^2
 * Range:     u8 n <= 21, u16 n <= 323, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t sum = 0;
    for (idx_t m = 2; m <= n; ++m) {
        seq_t v = (seq_t)(n / m);
        sum += v * v;
    }
    return sum;
}
