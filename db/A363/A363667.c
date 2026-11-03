/*
 * A363667  a(n) = Sum_{d|n} (n/d)^(n-n/d) * binomial(d+n-2,n-1).
 * https://oeis.org/A363667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(n-n/d) * binomial(d+n-2, n-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 533 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for 0, return 0 */
    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            idx_t nd = n / d;
            seq_t pow = ipow((seq_t)nd, (seq_t)(n - nd));
            seq_t bin = binomial(d + n - 2, n - 1);
            total += pow * bin;

            /* paired divisor n/d if distinct */
            if (d != nd) {
                idx_t d2 = nd;           /* n/d */
                idx_t nd2 = n / d2;      /* = d */
                seq_t pow2 = ipow((seq_t)nd2, (seq_t)(n - nd2));
                seq_t bin2 = binomial(d2 + n - 2, n - 1);
                total += pow2 * bin2;
            }
        }
    }
    return total;
}
