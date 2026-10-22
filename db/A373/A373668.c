/*
 * A373668  Expansion of e.g.f. exp(x / (1 - x^2)^3).
 * https://oeis.org/A373668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} binomial(3*n-5*k-1, k) /(n-2*k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum   = 0;
    seq_t ratio = 1;                 /* ratio = n!/(n-2*k)! for current k (k=0 ⇒ 1) */
    idx_t maxk  = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t b = binomial(3 * n - 5 * k - 1, k);   /* binomial(3*n‑5*k‑1, k) */
        sum += b * ratio;

        if (k == maxk)
            break;                           /* no next ratio when k==maxk */

        /*   ratio_{k+1} = ratio_k * (n-2k) * (n-2k-1)   */
        ratio *= (seq_t)(n - 2 * k);
        ratio *= (seq_t)(n - 2 * k - 1);
    }
    return sum;
}
