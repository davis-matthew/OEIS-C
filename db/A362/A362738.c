/*
 * A362738  a(n) is the least nonnegative integer solution for y such that x >
 *          1 is an integer in the equation n^y*x^n = n^(x^(1/n)).
 * https://oeis.org/A362738
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n - n^3 for n > 2; a(2)=a(3)=0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 3) {
        return 0;
    }
    seq_t base = (seq_t)n;
    seq_t pow_n = seq_pow(base, n);   /* n^n   */
    seq_t pow3  = seq_pow(base, (idx_t)3); /* n^3   */
    return pow_n - pow3;
}
