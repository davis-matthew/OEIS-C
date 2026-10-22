/*
 * A321564  a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^8.
 * https://oeis.org/A321564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d + n/d) * d^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 14, i64 n <= 234,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* compute x^8 as seq_t */
static seq_t pow8(seq_t x)
{
    seq_t r = 1;
    r *= x; r *= x; r *= x; r *= x;
    r *= x; r *= x; r *= x; r *= x;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)   /* iterate over divisors up to sqrt(n) */
    {
        if (n % d != 0) continue;

        idx_t e = n / d;                 /* complementary divisor */
        seq_t val_d = pow8((seq_t)d);
        int sign = (( (d & 1) ^ (e & 1) ) ? -1 : 1);  /* parity of d+e */

        if (sign > 0) {
            sum += val_d;
            if (e != d) sum += pow8((seq_t)e);
        } else {
            sum -= val_d;
            if (e != d) sum -= pow8((seq_t)e);
        }
    }
    return sum;
}
