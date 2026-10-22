/*
 * A362568  E.g.f. satisfies A(x) = exp(x/A(x)^x).
 * https://oeis.org/A362568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (-1)^k * (n‑k)^k *
 *            binomial(n‑k‑1,k)/(n‑k)!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 is a special case (avoids negative index in binomial) */
    if (n == 0) return 1;

    idx_t maxk = n / 2;            /* floor(n/2) */
    seq_t sum = 0;
    seq_t P = 1;                   /* product (n‑k+1)…*… for the current k */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t powv = pow_seq((seq_t)(n - k), (int)k);       /* (n-k)^k   */
        seq_t bin  = binomial(n - k - 1, k);                 /* binomial(n-k-1,k) */
        seq_t term = powv * bin;                            /* (n-k)^k * bin */
        term = term * P;                                    /* * P_k       */
        if (k & 1) term = -term;                            /* (-1)^k * ... */
        sum = sum + term;                                   /* accumulate */

        /* Prepare P for the next iteration:  P_{k+1} = P_k * (n-k) */
        if (k < maxk) {
            P = P * ((seq_t)(n - k));
        }
    }
    return sum;
}
