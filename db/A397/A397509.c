/*
 * A397509  a(n) = Sum_{k=0..n} binomial(n+2*k-3,k).
 * https://oeis.org/A397509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n + 2*k - 3, k) (OEIS A397509)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Compute a(n) for n ≥ 0 */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute N = n + 2*k - 3, may become negative for small n */
        int64_t N = (int64_t)n + 2 * (int64_t)k - 3;

        if (N < 0 && k > 0)
            /* binomial(N,k) = 0 for N < 0 and k > 0 */
            continue;

        /* For k == 0 and N < 0 we still need the term 1 */
        seq_t term;
        if (k == 0) {
            term = 1;
        } else {
            term = binomial((idx_t)N, k);
        }

        sum += term;
    }

    return sum;
}
