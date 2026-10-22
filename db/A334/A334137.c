/*
 * A334137  a(n) is the multiplicative inverse of A008514(n+1) modulo
 *          A008514(n).
 * https://oeis.org/A334137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = inverse of ( (n+1)^4 + (n+2)^4 ) modulo ( n^4 + (n+1)^4 )
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 219, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t modinv_u64(seq_t a, seq_t m)
{
    if (m <= (seq_t)1) return (seq_t)0;        /* inverse modulo 1 is 0 */

    unsigned long long aa = (unsigned long long)a;
    unsigned long long mm = (unsigned long long)m;

    unsigned long long r0 = aa, r1 = mm;
    __int128 x0 = 1, x1 = 0;
    /* y0, y1 are not used in the final result, but kept to keep the
       extended Euclidean algorithm complete. */
    __int128 y0 = 0, y1 = 1;

    while (r1 != 0) {
        unsigned long long q = r0 / r1;
        unsigned long long r2 = r0 % r1;
        __int128 x2 = x0 - (__int128)q * x1;
        __int128 y2 = y0 - (__int128)q * y1;
        r0 = r1; r1 = r2;
        x0 = x1; x1 = x2;
        y0 = y1; y1 = y2;
    }

    if (r0 != 1) return (seq_t)0;            /* no inverse exists – should not happen */

    __int128 inv = x0 % (__int128)mm;
    if (inv < 0) inv += (__int128)mm;
    return (seq_t)inv;
}

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;

    /* n^4 */
    seq_t n4 = sn * sn * sn * sn;

    /* (n+1)^4 */
    seq_t n1  = sn + 1;
    seq_t n14 = n1 * n1 * n1 * n1;

    /* Denominator: n^4 + (n+1)^4 */
    seq_t den = n4 + n14;

    /* (n+2)^4 */
    seq_t n2  = sn + 2;
    seq_t n24 = n2 * n2 * n2 * n2;

    /* Numerator: (n+1)^4 + (n+2)^4 */
    seq_t num = n14 + n24;

    return modinv_u64(num, den);
}
