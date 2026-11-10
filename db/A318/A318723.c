/*
 * A318723  Let f(0) = 0 and f(t*4^k + u) = i^t * ((1+i) * 2^k - f(u)) for any
 *          t in {1, 2, 3} and k >= 0 and u such that 0 <= u < 4^k (i denoting
 *          the imaginary unit); for any n >= 0, let g(n) = (f(A042968(n)) - 1
 *          - i) / 2; a(n) is the imaginary part of g(n).
 * https://oeis.org/A318723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   f(0)=0, f(t*4^k+u) = i^t*((1+i)*2^k - f(u)), t in {1,2,3},
 *            0<=u<4^k; a(n) = Im((f(A042968(n)) - 1 - i)/2), with A042968(n)
 *            = 4*floor(n/3) + (n%3) + 1
 * Range:     i8 n <= 12287, i16 n <= 12287, i32 n <= 12287, i64 n <= 12287,
 *            i128 n <= 12287, big any n
 * Validated: standard, pass, 92 terms through n = 9863 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct { seq_t re; seq_t im; } A318723_Complex;

static A318723_Complex A318723_f_of(uint64_t m)
{
    A318723_Complex z;
    if (m == 0) {
        z.re = 0;
        z.im = 0;
        return z;
    }

    uint64_t pow4 = 1;
    int k = 0;
    while (pow4 <= m / 4) { pow4 <<= 2; ++k; }

    uint64_t t = m / pow4;
    uint64_t u = m % pow4;

    A318723_Complex fu = A318723_f_of(u);

    seq_t two_pow_k = ((seq_t)1) << k;
    seq_t tmp_re = two_pow_k - fu.re;
    seq_t tmp_im = two_pow_k - fu.im;

    switch ((unsigned)t) {
        case 1: z.re = -tmp_im; z.im =  tmp_re; break;
        case 2: z.re = -tmp_re; z.im = -tmp_im; break;
        case 3: z.re =  tmp_im; z.im = -tmp_re; break;
        default: z.re = tmp_re; z.im = tmp_im; break;
    }
    return z;
}

seq_t a(idx_t n)
{
    uint64_t q = (uint64_t)(n / 3);
    uint64_t r = (uint64_t)(n % 3);
    uint64_t m = 4 * q + r + 1;

    A318723_Complex f = A318723_f_of(m);
    return (f.im - 1) / 2;
}
