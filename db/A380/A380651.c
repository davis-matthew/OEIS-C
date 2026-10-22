/*
 * A380651  a(n) = 4^n - n*3^(n-1).
 * https://oeis.org/A380651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - n*3^(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t pow4n   = pow4(n);
    seq_t pow3n_1 = pow3(n - 1);
    seq_t term    = (seq_t)n * pow3n_1;
    return pow4n - term;
}
