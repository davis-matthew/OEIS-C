/*
 * A376107  Expansion of e.g.f. LambertW(x / (1 - 3*x)).
 * https://oeis.org/A376107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376107: Expansion of e.g.f. LambertW(x/(1-3*x))
   Computed via Lagrange inversion:
   a(n) = (n-1)! * Σ_{k=0}^{n-1} binom(n,k) * 3^k *
          (- (n-k))^{n-1-k} / (n-1-k)!                     */
#include <stdbool.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum_pos = 0;   /* accumulated positive terms   */
    seq_t sum_neg = 0;   /* accumulated negative terms   */

    seq_t pow3    = 1;   /* 3^k               */
    seq_t falling = 1;   /* (n-1)!/(n-1-k)!   */
    seq_t binom   = 1;   /* binom(n,k)        */

    for (idx_t k = 0; k < n; ++k) {
        idx_t e    = n - 1 - k;            /* exponent for (- (n-k))  */
        idx_t base = n - k;                /* positive base          */

        /* absolute value of (-base)^e     */
        seq_t abs_pow = pow_seq((seq_t)base, (int)e);

        /* build the term magnitude        */
        seq_t term = abs_pow;
        term = term * binom;
        term = term * pow3;
        term = term * falling;

        /* sign determined by parity of e   */
        if (e & 1)      /* odd exponent → negative   */
            sum_neg += term;
        else
            sum_pos += term;

        /* prepare for next k (if any)      */
        if (k + 1 < n) {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            pow3 *= 3;
            if ((n - k - 1) > 0)
                falling *= (seq_t)(n - k - 1);
        }
    }

    return sum_pos - sum_neg;
}
