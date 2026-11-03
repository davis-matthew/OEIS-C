/*
 * A388535  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+4*k,k).
 * https://oeis.org/A388535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+4*k,k) (OEIS
 *            A388535)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 429 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;      /* accumulator for the sum */
    seq_t Cnk   = 1;      /* C(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = C(n,k) * C(n+4k,k) */
        seq_t term = Cnk * binomial(n + 4 * k, k);
        sum += term;

        /* update C(n,k+1) from C(n,k) for next iteration */
        if (k < n) {
            Cnk = Cnk * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    return sum;
}
