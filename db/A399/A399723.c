/*
 * A399723  a(n) is the greatest k <= n such that k divides sigma(n), where
 *          sigma = A000203.
 * https://oeis.org/A399723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = max{k ≤ n : k divides σ(n)}, where σ is the divisor sum
 *            function
 * Range:     u8 n <= 286, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, but graceful */

    seq_t sigma = sigma_of(n);

    for (idx_t k = n; k > 0; --k) {
        seq_t kseq = (seq_t)k;
        if (sigma % kseq == 0)
            return kseq;
    }

    return (seq_t)1;  /* should never be reached for n ≥ 1 */
}
