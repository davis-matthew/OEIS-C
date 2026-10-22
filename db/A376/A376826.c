/*
 * A376826  Array read by antidiagonals: T(n,k) = n! * [x^n] exp(2*x +
 *          (k/2)*x^2), n >= 0, k >= 0.
 * https://oeis.org/A376826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is the n‑th entry read by
 *            antidiagonals of the array T(n,k) = n!·[xⁿ]exp(2x+(k/2)x²)
 * Range:     u8 n <= 32, u16 n <= 73, u32 n <= 165, u64 n <= 399,
 *            u128 n <= 1070, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the explicit summation
   T(n,k) = Σ_{t=0..⌊n/2⌋} binomial(n,2t)·2^(n-2t)·k^t·(2t-1)!!      */

/* Include required utility functions */
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* --- Step 1: find the diagonal (i + j) and the position on it --- */
    idx_t d = 0; /* diagonal number (sum of indices) */
    while ((d + 1) * (d + 2) / 2 <= n) {
        ++d;
    }
    /* (d*(d+1)/2) is the number of terms before this diagonal */
    idx_t pos   = n - d * (d + 1) / 2;   /* position within the diagonal 0..d */
    idx_t i     = pos;                   /* row index (n) */
    idx_t j     = d - pos;               /* column index (k) */

    /* --- Step 2: compute T(i,j) via the summation formula --- */
    seq_t sum = 0;
    seq_t kpow  = 1;    /* k^t, grows with t */
    seq_t odd   = 1;    /* (2t-1)!!, grows with t */

    /* For each t from 0 to floor(i/2) compute the term */
    for (idx_t t = 0; t * 2 <= i; ++t) {
        /* binomial(i,2t) */
        seq_t bin = binomial(i, 2 * t);

        /* 2^(i-2t) */
        seq_t pow2_val = pow2(i - 2 * t);

        /* k^t is maintained in kpow */
        /* (2t-1)!! is maintained in odd */

        /* term = bin * pow2_val * kpow * odd */
        seq_t term = bin;
        term = term * pow2_val;
        term = term * kpow;
        term = term * odd;

        sum += term;

        /* update kpow and odd for next t+1 */
        kpow = kpow * (seq_t)j;                 /* multiply by k */
        if (t + 1 > 0) {
            idx_t add = 2 * (t + 1) - 1;           /* next odd factor */
            odd = odd * (seq_t)add;
        }
    }

    return sum;
}
