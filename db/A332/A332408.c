/*
 * A332408  a(n) = Sum_{k=0..n} binomial(n,k) * k! * k^n.
 * https://oeis.org/A332408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * k! * k^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 222 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t C   = 1;   /* C = binomial(n, k)     */
    seq_t fact = 1;  /* fact = k!              */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t kpow = ipow((seq_t)k, (seq_t)n);  /* k^n  */
        seq_t term = C * fact;
        term = term * kpow;
        sum += term;

        if (k < n) /* update C and fact for next k */
        {
            C = C * ((seq_t)(n - k)) / ((seq_t)(k + 1));
            fact *= (seq_t)(k + 1);
        }
    }
    return sum;
}
