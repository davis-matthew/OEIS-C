/*
 * A318704  For any n >= 0 with binary expansion Sum_{k=0..w} b_k * 2^k, let
 *          f(n) = Sum_{k=0..w} b_k * i^k * 2^floor(k/2) (where i denotes the
 *          imaginary unit); a(n) is the square of the modulus of f(n).
 * https://oeis.org/A318704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = sum_{k even} (b_k * (-1)^{k/2} * 2^{k/2})^2 + sum_{k odd}
 *            (b_k * (-1)^{(k-1)/2} * 2^{(k-1)/2})^2
 * Range:     u8 n <= 255, u16 n <= 9611, u32 n <= 9611, u64 n <= 9611,
 *            u128 n <= 9611, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>

seq_t a(idx_t n)
{
    int64_t real = 0, imag = 0;
    uint64_t k = 0;
    uint64_t tmp = n;
    while (tmp != 0) {
        if (tmp & 1) {
            uint64_t m = k >> 1;                     /* floor(k/2) */
            int64_t sign = (m & 1) ? -1 : +1;
            int64_t val = sign * ((int64_t)1 << m); /* signed power */
            if ((k & 1) == 0) {                     /* even k -> real part */
                real += val;
            } else {                                /* odd k -> imag part */
                imag += val;
            }
        }
        tmp >>= 1;
        ++k;
    }

    /* compute |f(n)|^2 = real^2 + imag^2, all signed-converted to unsigned */
    __int128 rr = (__int128)real * (__int128)real;
    __int128 ii = (__int128)imag * (__int128)imag;
    unsigned __int128 res = (unsigned __int128)rr + (unsigned __int128)ii;
    return (seq_t)res;
}
