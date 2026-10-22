/*
 * A369401  Expansion of (1/x) * Series_Reversion( x / (1+x) * (1-x^3)^3 ).
 * https://oeis.org/A369401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(3*n + k + 2, k) *
 *            binomial(n+1, n-3*k) (OEIS A369401)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t is provided by the build system; it may be uint8_t … uint64_t,
   unsigned __int128, or an arbitrary‑precision integer class.  */
seq_t a(idx_t n)
{
    /* number of terms in the sum */
    const idx_t kmax = n / 3;

    /* binomial values for the current k
       binom_f  = binomial(3*n + k + 2, k)
       binom_r  = binomial(n+1, n-3*k)   //  = binomial(n+1, 3*k+1)   */
    seq_t binom_f = 1;                   /* k = 0  →  binom(3*n+2, 0) = 1      */
    seq_t binom_r = (seq_t)(n + 1);       /* k = 0  →  binom(n+1, 1) = n+1      */

    seq_t sum = 0;                       /* accumulates the numerator */

    for (idx_t k = 0; k <= kmax; ++k) {
        /* add the current term */
        sum += binom_f * binom_r;

        if (k == kmax) break;

        /* update binom_f to binomial(3*n + (k+1) + 2, k+1)   */
        binom_f = binom_f *
            (seq_t)(3 * n + k + 3) /
            (seq_t)(k + 1);

        /* update binom_r to binomial(n+1, 3*(k+1)+1) */
        idx_t r = 3 * k + 1;                     /* 3*k + 1  = current r    */
        seq_t tmp = binom_r *
            (seq_t)(n + 1 - r) *
            (seq_t)(n + 1 - r - 1) *
            (seq_t)(n + 1 - r - 2);
        tmp = tmp / (seq_t)(r + 1);
        tmp = tmp / (seq_t)(r + 2);
        tmp = tmp / (seq_t)(r + 3);
        binom_r = tmp;
    }

    /* final division by (n+1) */
    return sum / (seq_t)(n + 1);
}
