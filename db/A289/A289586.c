/*
 * A289586  Numbers k whose smallest multiple that is a Fibonacci number is
 *          Fibonacci(k).
 * https://oeis.org/A289586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2k)=5^k (k≥1), a(2k−1)=12·5^(k−2) (k≥2)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/mul_small.h" /* mul_small(seq_t x, unsigned int m) */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;              /* a(1)=1 */

    if (n & (idx_t)1)                 /* n is odd: n = 2k−1, k≥2 */
    {
        idx_t exp = (n - (idx_t)3) / (idx_t)2;   /* k−2 */
        seq_t val  = seq_pow((seq_t)5, exp);      /* 5^(k−2) */
        return mul_small(val, (unsigned)12);      /* 12·5^(k−2) */
    }
    else                               /* n is even: n = 2k, k≥1 */
    {
        idx_t exp = n / (idx_t)2;            /* k */
        return seq_pow((seq_t)5, exp);       /* 5^k */
    }
}
