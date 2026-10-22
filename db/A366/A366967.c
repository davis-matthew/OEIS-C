/*
 * A366967  a(n) = Sum_{k=2..n} binomial(k,2) * floor(n/k).
 * https://oeis.org/A366967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} binomial(k,2) * floor(n/k)
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) { 
    if (n < 2)
        return 0;
    seq_t sum = 0;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t bin = (seq_t)k * (k - 1) / 2;
        sum += bin * (seq_t)(n / k);
    }
    return sum;
}
