/*
 * A356248  Image of 1 under repeated application of the map k ->
 *          (2k-1,2k,2k-1).
 * https://oeis.org/A356248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{i: digit i of n in base 3 equals 1} 2^i
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t pos    = 0;
    while (n > 0) {
        if (n % (idx_t)3 == (idx_t)1) {
            result += ((seq_t)1 << pos);
        }
        n /= (idx_t)3;
        pos++;
    }
    return result;
}
