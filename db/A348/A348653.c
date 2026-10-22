/*
 * A348653  For any nonnegative number n with base-13 expansion Sum_{k >= 0}
 *          d_k*13^k, a(n) is the imaginary part of Sum_{k >= 0}
 *          g(d_k)*(3+2*i)^k where g(0) = 0, and g(1+u+3*v) = (1+u*i)*i^v for
 *          any u = 0..2 and v = 0..3 (where i denotes the imaginary unit);
 *          see A348652 for the real part.
 * https://oeis.org/A348653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) is the imaginary part of Σ g(d_k)·(3+2i)^k, where g(0)=0
 *            and g(1+u+3v)=(1+u*i)·i^v for 0≤u≤2, 0≤v≤3
 * Range:     i8 n <= 2197, i16 n <= 2197, i32 n <= 2197, i64 n <= 2197,
 *            i128 n <= 2197, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t sum_real = 0;
    seq_t sum_imag = 0;
    seq_t pow_real = 1;   /* (3+2i)^0 */
    seq_t pow_imag = 0;

    while (n > 0) {
        unsigned int d = n % 13;      /* base‑13 digit */
        n /= 13;

        seq_t g_real, g_imag;
        if (d == 0) {
            g_real = 0;
            g_imag = 0;
        } else {
            unsigned int t = d - 1;   /* 0 ≤ t ≤ 11 */
            unsigned int u = t % 3;   /* 0 to 2 */
            unsigned int v = t / 3;   /* 0 to 3 */
            switch (v) {
                case 0: g_real = 1;          g_imag = u;          break;
                case 1: g_real = -((seq_t)u); g_imag = 1;          break;
                case 2: g_real = -1;         g_imag = -((seq_t)u); break;
                case 3: g_real = u;          g_imag = -1;         break;
            }
        }

        /* Product = g(d) * (pow_real + pow_imag·i) */
        seq_t prod_real = g_real * pow_real - g_imag * pow_imag;
        seq_t prod_imag = g_real * pow_imag + g_imag * pow_real;

        sum_real += prod_real;
        sum_imag += prod_imag;

        /* Update (3+2i)^(k+1) = (3+2i) * (pow_real + pow_imag·i) */
        seq_t next_real = 3 * pow_real - 2 * pow_imag;
        seq_t next_imag = 2 * pow_real + 3 * pow_imag;
        pow_real = next_real;
        pow_imag = next_imag;
    }

    return sum_imag;
}
