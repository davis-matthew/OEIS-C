/*
 * A294409  a(n) = n! * [x^n] exp(n*x)*BesselI(0,2*n*x).
 * https://oeis.org/A294409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n · ∑_{k=0}^{⌊n/2⌋} C(n,2k) · C(2k,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = coefficient of x^n in (1 + n·x + n²·x²)^n          */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset 0 explicitly; 0^0 is defined as 1 */
    if (n == 0) return 1;

    /* sum_{k=0}^{⌊n/2⌋} C(n,2k)·C(2k,k) */
    seq_t sum  = 0;
    seq_t term = 1;                /* term for k = 0  (C(n,0)·C(0,0) = 1) */
    idx_t maxk = n >> 1;           /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        sum += term;

        /* Update term for the next k (if any)
           term_{k+1} = term_k · (n-2k)·(n-2k-1) / ((k+1)^2)   */
        if (k < maxk) {
            term *= (seq_t)(n - 2 * k);
            term /= (seq_t)(k + 1);
            term *= (seq_t)(n - 2 * k - 1);
            term /= (seq_t)(k + 1);
        }
    }

    /* Multiply by n^n */
    seq_t pow = big_pow((seq_t)n, (seq_t)n);   /* n^n */
    return sum * pow;
}
