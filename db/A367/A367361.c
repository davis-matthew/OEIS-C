/*
 * A367361  Comma transform of powers of 2.
 * https://oeis.org/A367361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*A000689(n) + A008952(n+1) * where A000689(n) = 2^n mod
 *            10 and * A008952(m) = leading decimal digit of 2^m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* 2^n mod 10 */
    seq_t m;
    if (n == 0) {
        m = 1;
    } else {
        switch (n & 3) {   /* n % 4 */
            case 1:  m = 2; break;
            case 2:  m = 4; break;
            case 3:  m = 8; break;
            default: m = 6; break;  /* n % 4 == 0 for n >= 4 */
        }
    }

    /* leading decimal digit of 2^(n+1) */
    static const long double log10_2 =
        0.3010299956639811952137388947244930L;
    long double x = ((long double)n + 1.0L) * log10_2;
    long double frac = x - floorl(x);
    long double pow10_frac = powl(10.0L, frac);
    int d = (int)(pow10_frac + 1e-12L);  /* floor */

    return m * 10 + d;
}
