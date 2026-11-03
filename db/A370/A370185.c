/*
 * A370185  Coefficient of x^n in the expansion of (1+x+x^3)^(2*n).
 * https://oeis.org/A370185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = \sum_{k=0}^{\lfloor n/3 \rfloor} \binom{2n}{k} \cdot
 *            \binom{2n-k}{n-3k}
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t max_k = n / 3;
    seq_t total = 0;
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t c1 = binomial(2*n, k);
        seq_t c2 = binomial(2*n - k, n - 3*k);
        total += c1 * c2;
    }
    return total;
}
