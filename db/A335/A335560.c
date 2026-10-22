/*
 * A335560  Number of ways to tile an n X n square with 1 X 1 squares and
 *          (n-1) X 1 vertical or horizontal strips.
 * https://oeis.org/A335560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^n + 12*2^n - 19, for n >= 3. a(1) = 1, a(2) = 16
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)16;
    seq_t pow3n = pow3(n);
    seq_t pow2n = pow2(n);
    seq_t result = pow3n * (seq_t)2;
    result += pow2n * (seq_t)12;
    result -= (seq_t)19;
    return result;
}
