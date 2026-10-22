/*
 * A394495  Expansion of e.g.f. (1/x) * Series_Reversion( x*(1 - 5*x)^(1/5) ).
 * https://oeis.org/A394495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=1}^{n-1} (6*n + 1 - 5*k), with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_mul.h"    /* provides seq_mul for multiplying seq_t values */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;      /* a(0) = 1 */

    seq_t result = (seq_t)1;
    /* Compute product of terms (6*n + 1 - 5*k) for k = 1 .. n-1 */
    for (idx_t k = 1; k < n; ++k) {
        seq_t term = (seq_t)(6ULL * n + 1 - 5ULL * k); /* terms fit in 64‑bit for typical n */
        result = seq_mul(result, term);
    }
    return result;
}
