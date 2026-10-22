/*
 * A373681  Expansion of e.g.f. exp(x / (1 - x^2)^2) / (1 - x^2).
 * https://oeis.org/A373681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} binomial(2*n-3*k,k)/(n-2*k)!
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t kmax = n / 2;                /* floor(n/2) */
    seq_t prod   = 1;                  /* falling factorial n!/(n-2k)! */
    seq_t binom  = 1;                  /* B(k) = C(2n-3k, k)           */
    seq_t sum    = 0;

    for (idx_t k = 0; k <= kmax; ++k) {
        sum += prod * binom;            /* add current term */

        if (k == kmax)                /* last k visited */
            break;

        /* update prod for next k: multiply by two new factors */
        idx_t a = n - 2 * k;           /* n-2k   */
        idx_t b = a - 1;               /* n-2k-1 */
        prod *= (seq_t)a * (seq_t)b;

        /* update binom for next k using exact recurrence */
        idx_t kk = k + 1;                        /* new k      */
        idx_t Q  = 2 * n - 4 * kk;               /* Q = 2n-4k   */
        seq_t num = (seq_t)(Q + 4) * (seq_t)(Q + 3) *
                    (seq_t)(Q + 2) * (seq_t)(Q + 1);

        idx_t P  = 2 * n - 3 * kk;               /* P = 2n-3k   */
        seq_t den = (seq_t)kk * (seq_t)(P + 3) *
                    (seq_t)(P + 2) * (seq_t)(P + 1);

        binom = binom * num / den;              /* exact division */
    }
    return sum;
}
