/*
 * A348761  For any nonnegative number n with binary expansion Sum_{k >= 0}
 *          b_k * 2^k, a(n) is the imaginary part of f(n) = Sum_{k >= 0}
 *          ((-1)^Sum_{j = 0..k-1} b_j) * (1+i)^k (where i denotes the
 *          imaginary unit); sequence A348760 gives the real part.
 * https://oeis.org/A348761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = Im( Σ_{k ≥ 0} b_k * (-1)^{Σ_{j=0}^{k-1} b_j} * (1+i)^k )
 * Range:     i8 n <= 8191, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where b_k are the binary digits of n (b_0 is LSB). */

#include <stdint.h>  /* for shift operations, may be optional */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Determine index of highest set bit (msb). */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp) { tmp >>= 1; ++msb; }
    msb--;  /* msb is zero‑based */

    seq_t real = 1;   /* (1+i)^0 real part */
    seq_t imag = 0;   /* (1+i)^0 imag part */
    unsigned parity = 0;   /* parity of popcount of bits seen so far */
    seq_t sum = 0;    /* accumulation of imaginary part */

    for (idx_t k = 0; k <= msb; ++k) {
        unsigned bit = (n >> k) & 1U;
        if (bit) {
            /* sign = (-1)^parity */
            if (parity == 0)
                sum += imag;
            else
                sum -= imag;
        }

        /* Advance (1+i)^k to (1+i)^(k+1) */
        seq_t tmp_real = real;
        seq_t tmp_imag = imag;
        real = tmp_real - tmp_imag;
        imag = tmp_real + tmp_imag;

        if (bit)
            parity ^= 1U;   /* flip parity if current bit is 1 */
    }

    return sum;
}
