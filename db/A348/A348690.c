/*
 * A348690  For any nonnegative number n with binary expansion Sum_{k >= 0}
 *          b_k * 2^k, a(n) is the real part of f(n) = Sum_{k >= 0} b_k *
 *          (i^Sum_{j = 0..k-1} b_j) * (1+i)^k (where i denotes the imaginary
 *          unit); sequence A348691 gives the imaginary part.
 * https://oeis.org/A348690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = Re ( Σ_{k≥0} b_k · i^{Σ_{j=0}^{k-1} b_j} · (1+i)^k )
 * Range:     i8 n <= 8191, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where n = Σ_{k≥0} b_k · 2^k  (binary expansion)                      */

#include <stdint.h>

/* Computes a(n): the real part of the complex sum defined above.   */
seq_t a(idx_t n) {
    seq_t acc_real = 0, acc_imag = 0;    /* accumulator for the sum   */
    seq_t pow_real = 1, pow_imag = 0;    /* (1+i)^k   current k      */
    unsigned int ipwr = 0;               /* index c = Σ_{j<k} b_j (mod 4) */

    idx_t x = n;                        /* iterate over bits of n    */

    while (x != 0) {
        /* If the current bit is 1, add the term     */
        if (x & 1) {
            seq_t term_real, term_imag;
            switch (ipwr & 3) {
                case 0: term_real =  pow_real;  term_imag =  pow_imag; break;
                case 1: term_real = -pow_imag;  term_imag =  pow_real; break;
                case 2: term_real = -pow_real;  term_imag = -pow_imag; break;
                case 3: term_real =  pow_imag;  term_imag = -pow_real; break;
            }
            acc_real += term_real;
            acc_imag += term_imag;
        }

        /* Update for the next bit position          */
        if (x & 1) {
            ipwr = (ipwr + 1) & 3;            /* increment power of i   */
        }

        /* pow = pow * (1+i)  -> new_real = real - imag,
           new_imag = real + imag                       */
        seq_t new_real = pow_real - pow_imag;
        seq_t new_imag = pow_real + pow_imag;
        pow_real = new_real;
        pow_imag = new_imag;

        x >>= 1;      /* move to next bit                      */
    }

    return acc_real;
}
