/*
 * A387814  a(n) = Sum_{k=0..n} binomial(6*n+1,6*k).
 * https://oeis.org/A387814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2*64^n + 3*(-27)^n)/6 (n >= 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 64^n and 27^n */
    seq_t pow64 = big_pow((seq_t)64, (seq_t)n);
    seq_t pow27 = big_pow((seq_t)27, (seq_t)n);

    /* 1 + 2*64^n */
    seq_t base = pow64 * (seq_t)2 + (seq_t)1;

    /* 3*(-27)^n : add for even n, subtract for odd n */
    seq_t term = pow27 * (seq_t)3;
    seq_t sum  = (n & 1) ? (base - term) : (base + term);

    /* Exact division by 6 */
    return sum / (seq_t)6;
}
