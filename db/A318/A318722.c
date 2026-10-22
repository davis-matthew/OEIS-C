/*
 * A318722  Let f(0) = 0 and f(t*4^k + u) = i^t * ((1+i) * 2^k - f(u)) for any
 *          t in {1, 2, 3} and k >= 0 and u such that 0 <= u < 4^k (i denoting
 *          the imaginary unit); for any n >= 0, let g(n) = (f(A042968(n)) - 1
 *          - i) / 2; a(n) is the real part of g(n).
 * https://oeis.org/A318722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 9283, i16 n <= 9283, i32 n <= 9283, i64 n <= 9283,
 *            i128 n <= 9283, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A318722:  a(n) = Re((f(A042968(n)) - 1 - i)/2)
 *  where   f(0) = 0
 *          f(t*4^k + u) = i^t * ((1+i)*2^k - f(u)),  t∈{1,2,3},
 *  and  A042968(n) = 4*floor(n/3)+((n%3)+1)  (numbers not divisible by 4)
 */

#include <stdint.h>

typedef struct { seq_t re; seq_t im; } Complex;

/* recursive computation of f(m) for m ≥ 0 */
static Complex f_of(uint64_t m)
{
    if (m == 0) return (Complex){0, 0};

    /* find largest power of 4 ≤ m and its exponent k */
    uint64_t pow4 = 1;
    int k = 0;
    while (pow4 <= m / 4) { pow4 <<= 2; ++k; }

    uint64_t t = m / pow4;          /* leading base‑4 digit 1–3 */
    uint64_t u = m % pow4;          /* lower part */

    Complex fu = f_of(u);

    /* (1+i)*2^k  ->  real = 2^k, imag = 2^k */
    seq_t two_pow_k = ((seq_t)1) << k;
    seq_t tmp_re = two_pow_k - fu.re;
    seq_t tmp_im = two_pow_k - fu.im;

    /* multiply by i^t */
    seq_t re, im;
    switch ((unsigned)t) {
        case 1: re = -tmp_im; im =  tmp_re; break;
        case 2: re = -tmp_re; im = -tmp_im; break;
        case 3: re =  tmp_im; im = -tmp_re; break;
        default: re =  tmp_re; im =  tmp_im; /* should not happen for m>0 */
    }
    return (Complex){re, im};
}

seq_t a(idx_t n)
{
    /* A042968(n) = 4*floor(n/3) + ((n%3)+1) */
    idx_t q = n / 3;
    idx_t r = n % 3;
    uint64_t m = 4*q + r + 1;

    Complex f = f_of(m);
    return (f.re - 1) / 2;
}
