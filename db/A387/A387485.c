/*
 * A387485  a(n) = Sum_{k=0..floor(n/3)} 2^(n-2*k) * binomial(k,n-3*k)^2.
 * https://oeis.org/A387485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} 2^(n-2k) * binomial(k, n-3k)^2
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 133,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binomial(k, r) is computed multiplicatively */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Quick handling of trivial cases */
    const idx_t start = (n + 3) / 4;     /* ceil(n/4) */
    const idx_t end   = n / 3;           /* floor(n/3) */
    if (start > end)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    for (idx_t k = start; k <= end; ++k) {
        /* r = n - 3*k  (0 <= r <= k by construction) */
        idx_t r = n - 3 * k;

        /* Compute binomial(k, r) multiplicatively */
        seq_t bin = (seq_t)1;
        idx_t nn = k;
        idx_t rr = r;
        if (rr > nn - rr)   /* use symmetry to reduce iterations */
            rr = nn - rr;

        for (idx_t i = (idx_t)1; i <= rr; ++i) {
            bin = bin * (nn - rr + i);
            bin = bin / i;
        }

        /* Compute contribution: 2^(n-2k) * bin^2 */
        seq_t term = pow2(n - 2 * k);
        term *= bin;
        term *= bin;

        sum += term;
    }

    return sum;
}
