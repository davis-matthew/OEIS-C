/*
 * A393336  a(n) is the sum of positions of 1's minus sum of positions of 0's
 *          in the binary expansion of n.
 * https://oeis.org/A393336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, base
 * Formula:   a(n) = sum_{i=1}^{k} (bit_i == 1 ? i : -i), where k =
 *            floor(log2(n))+1 and a(0) = -1. Here bit_i is the i-th least
 *            significant bit of n (i=1 at LSB)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)-1;
    }

    seq_t sum = 0;
    idx_t pos  = 1;
    idx_t temp = n;

    while (temp) {
        if (temp & 1)
            sum += (seq_t)pos;
        else
            sum -= (seq_t)pos;
        pos++;
        temp >>= 1;
    }

    return sum;
}
