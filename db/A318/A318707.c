/*
 * A318707  For any n >= 0 with base-9 representation Sum_{k=0..w} d_k * 9^k,
 *          let g(n) = Sum_{k=0..w} s(d_k) * 3^k (where s(0) = 0, s(1+2*j) =
 *          i^j and s(2+2*j) = i^j * (1+i) for any j > 0, and i denotes the
 *          imaginary unit); a(n) is the square of the modulus of g(n).
 * https://oeis.org/A318707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = |Sum_{k>=0} s(d_k) * 3^k|^2 where d_k are the digits of n
 *            in base 9 and s(0)=0, s(1)=1, s(2)=1+i, s(3)=i, s(4)=-1+i,
 *            s(5)=-1, s(6)=-1-i, s(7)=-i, s(8)=1-i
 * Range:     u8 n <= 172, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    /* Tables for the real and imaginary parts of s(d) for d = 0..8. */
    static const int8_t s_real[9] = { 0,  1,  1,  0, -1, -1, -1,  0,  1 };
    static const int8_t s_imag[9] = { 0,  0,  1,  1,  1,  0, -1, -1, -1 };

    /* Find the highest power of 9 that does not exceed n. */
    uint64_t pow9 = 1;
    while (pow9 <= n / 9)  /* pow9 * 9 <= n prevents overflow */
        pow9 *= 9;

    /* Accumulate the complex sum g = Sum s(d_k) * 3^k using the
       recurrence g = g * 3 + s(d) while scanning from most to least
       significant digits. */
    int64_t xr = 0, xi = 0;               /* real and imaginary parts of g */
    uint64_t remaining = n;

    while (pow9 > 0) {
        uint64_t d = remaining / pow9;        /* current digit */
        remaining %= pow9;                   /* remove processed digit */
        pow9 /= 9;                           /* move to next lower weight */

        int64_t sr = s_real[(int)d];
        int64_t si = s_imag[(int)d];

        xr = xr * 3 + sr;
        xi = xi * 3 + si;
    }

    /* Return the squared modulus |g|^2 = xr^2 + xi^2. */
    int64_t val = xr * xr + xi * xi;
    return (seq_t)val;
}
