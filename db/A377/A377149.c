/*
 * A377149  a(n) = Sum_{k=0..floor(n/2)} binomial(k+3,3) *
 *          binomial(k,n-2*k)^2.
 * https://oeis.org/A377149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(k+3,3) *
 *            binomial(k,n-2*k)^2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A377149: a(n) = Σ_{k=0}^{⌊n/2⌋} C(k+3,3) ⋅ [C(k,n-2k)]² */
/* Uses the binomial utility from the build. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n >> 1;               /* floor(n/2) */

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t m = n - (k << 1);       /* n - 2*k */

        if (m > k)                    /* binomial(k,m) would be 0 */
            continue;

        seq_t c1 = binomial(k + 3, 3);   /* C(k+3,3) */
        seq_t c2 = binomial(k, m);       /* C(k,n-2k) */
        seq_t term = c1 * c2 * c2;       /* C(k+3,3) * C(k,n-2k)^2 */
        sum += term;
    }

    return sum;
}
