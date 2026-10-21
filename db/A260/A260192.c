/*
 * A260192  Kronecker symbol(-6 / 2*n + 7).
 * https://oeis.org/A260192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Kronecker symbol(-6, 2*n + 7). For odd m = 2*n + 7, a(n)
 *            = (-1 | m) * (2 | m) * (3 | m), where (2 | m) = 1 if m≡±1 mod 8
 *            else -1, and (3 | m) = 1 if m≡±1 mod 12 else -1; if 3 divides m
 *            (i.e., m≡0 mod 3) then a(n)=0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 2*n + 7;
    if ((m % 3) == 0)
        return 0;

    int sign = 1;

    /* (-1 | m) : +1 if m≡1 mod 4, else -1 */
    sign *= ((m % 4) == 1) ? 1 : -1;

    /* (2 | m) : +1 if m≡±1 mod 8, else -1 */
    idx_t m8 = m & 7;  /* m % 8, faster for unsigned ints */
    sign *= (m8 == 1 || m8 == 7) ? 1 : -1;

    /* (3 | m) : +1 if m≡±1 mod 12, else -1 (m not divisible by 3) */
    idx_t m12 = m % 12;
    sign *= (m12 == 1 || m12 == 11) ? 1 : -1;

    return (seq_t)sign;
}
