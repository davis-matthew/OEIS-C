/*
 * A265276  Least real z > 2/3 such that 1/2 = Sum_{n>=1} {n*z} / 2^n, where
 *          {x} denotes the fractional part of x.
 * https://oeis.org/A265276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) * NUM/DEN) mod 10
 * Range:     u8 none, u16 none, u32 none, u64 n <= 199, u128 n <= 199,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (The digits of the rational approximation 728610523/1073741820, accurate to >5 million digits.) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* rational approximation of the constant z */
    static const seq_t NUM = (seq_t)728610523;
    static const seq_t DEN = (seq_t)1073741820;

    seq_t r = NUM;            /* current remainder */
    seq_t digit = 0;          /* digit to return */

    for (idx_t i = 0; i <= n; ++i) {
        r *= (seq_t)10;
        digit = r / DEN;
        r     = r % DEN;
    }
    return digit;
}
