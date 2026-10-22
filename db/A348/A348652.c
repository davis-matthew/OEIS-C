/*
 * A348652  For any nonnegative number n with base-13 expansion Sum_{k >= 0}
 *          d_k*13^k, a(n) is the real part of Sum_{k >= 0} g(d_k)*(3+2*i)^k
 *          where g(0) = 0, and g(1+u+3*v) = (1+u*i)*i^v for any u = 0..2 and
 *          v = 0..3 (where i denotes the imaginary unit); see A348653 for the
 *          imaginary part.
 * https://oeis.org/A348652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = real part of \sum_{k>=0} g(d_k)*(3+2i)^k * where d_k are
 *            base‑13 digits of n, g(0)=0, * g(1+u+3v)=(1+u*i)*i^v for 0≤u≤2,
 *            0≤v≤3
 * Range:     i8 n <= 2197, i16 n <= 2197, i32 n <= 2197, i64 n <= 2197,
 *            i128 n <= 2197, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>       // optional, for clarity

seq_t a(idx_t n)
{
    /* mapping of g(d) for d = 0..12: real and imaginary parts */
    static const int g_real[13] = {0,  1, 1, 1, 0, -1, -2, -1, -1, -1, 0,  1,  2};
    static const int g_imag[13] = {0,  0, 1, 2, 1,  1,  1,  0, -1, -2, -1, -1, -1};

    if (n == 0) return 0;

    seq_t real_sum = 0;
    seq_t imag_sum = 0;                /* kept for completeness, unused */
    seq_t pow_r   = 1;                 /* (3+2i)^0 real part   */
    seq_t pow_i   = 0;                 /* (3+2i)^0 imaginary   */
    const seq_t z_r = 3;               /* real part of 3+2i     */
    const seq_t z_i = 2;               /* imaginary part of 3+2i */

    idx_t temp = n;
    while (temp > 0) {
        int digit = (int)(temp % 13);
        temp /= 13;

        int gr = g_real[digit];
        int gi = g_imag[digit];

        /* multiply g by current power of (3+2i) */
        seq_t term_r = (seq_t)gr * pow_r - (seq_t)gi * pow_i;
        seq_t term_i = (seq_t)gr * pow_i + (seq_t)gi * pow_r;

        real_sum += term_r;
        imag_sum += term_i;    /* not returned, but harmless */

        /* update power: pow * (3+2i) */
        seq_t next_r = pow_r * z_r - pow_i * z_i;
        seq_t next_i = pow_r * z_i + pow_i * z_r;
        pow_r = next_r;
        pow_i = next_i;
    }

    return real_sum;
}
