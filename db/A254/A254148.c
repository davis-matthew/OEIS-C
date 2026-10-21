/*
 * A254148  a(n) = 9*2^n + 7*4^n + 3*8^n + 8*3^n + 2*9^n + 6*5^n + 5*6^n +
 *          4*7^n + 10^n + 10.
 * https://oeis.org/A254148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*2^n + 7*4^n + 3*8^n + 8*3^n + 2*9^n + 6*5^n + 5*6^n +
 *            4*7^n + 10^n + 10
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t t;

    t = pow_int((seq_t)2, n);  res += (seq_t)9 * t;
    t = pow_int((seq_t)4, n);  res += (seq_t)7 * t;
    t = pow_int((seq_t)8, n);  res += (seq_t)3 * t;
    t = pow_int((seq_t)3, n);  res += (seq_t)8 * t;
    t = pow_int((seq_t)9, n);  res += (seq_t)2 * t;
    t = pow_int((seq_t)5, n);  res += (seq_t)6 * t;
    t = pow_int((seq_t)6, n);  res += (seq_t)5 * t;
    t = pow_int((seq_t)7, n);  res += (seq_t)4 * t;
    t = pow_int((seq_t)10, n); res += t + (seq_t)10;

    return res;
}
