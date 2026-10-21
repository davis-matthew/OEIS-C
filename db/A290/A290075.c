/*
 * A290075  Number of monomials in c(n) where c(1) = x, c(2) = y, c(n+2) =
 *          c(n+1) + c(n)^2.
 * https://oeis.org/A290075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1, a(3)=2 * For n > 2: * if n is even: a(n) =
 *            2^(n/2-1) + 2^(n-4) * if n is odd: a(n) = 3*2^((n-5)/2) +
 *            2^(n-4) * * The implementation uses the helper pow_uint from the
 *            utils folder * to avoid undefined behaviour when the shift count
 *            may exceed the * width of the underlying type for...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)2;

    /* All other values use the closed form below. */
    if ((n & 1) == 0) {                /* even n > 2 */
        idx_t e1 = n / 2 - 1;
        idx_t e2 = n - 4;
        seq_t t1 = pow_uint(2, e1);
        seq_t t2 = pow_uint(2, e2);
        return t1 + t2;
    } else {                            /* odd n > 2 */
        idx_t e1 = (n - 5) / 2;
        idx_t e2 = n - 4;
        seq_t t1 = pow_uint(2, e1);    /* 2^((n-5)/2) */
        t1 *= (seq_t)3;                /* 3 * 2^((n-5)/2) */
        seq_t t2 = pow_uint(2, e2);
        return t1 + t2;
    }
}
