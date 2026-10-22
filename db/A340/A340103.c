/*
 * A340103  a(n) = [x^n] Product_{k>=1} (1 + n^(k-1)*x^k).
 * https://oeis.org/A340103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k≥1} (1 + n^(k-1) · x^k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (coefficient extraction from the product over all k). */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif
#include "../utils/seq_mul.h"

/* Compute a(n).  Works for any seq_t/idx_t type supplied by the build,
   and is independent of the order in which a(n) is called. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

#ifdef __cplusplus
    /* C++: use std::vector which default‑constructs each element. */
    std::vector<seq_t> coeff(static_cast<size_t>(n) + 1);
    coeff[0] = (seq_t)1;
    seq_t power = (seq_t)1;     /* n^(k‑1) for the current factor. */

    for (idx_t k = 1; k <= n; ++k) {
        /* update from high degree to low to avoid overwriting needed values */
        for (idx_t j = n; j >= k; --j) {
            coeff[j] = coeff[j] + seq_mul(coeff[j - k], power);
        }
        if (k < n) power = seq_mul(power, (seq_t)n);
    }
    return coeff[n];

#else
    /* C: allocate a zero‑initialized array of seq_t using calloc. */
    size_t sz = (size_t)n + 1;
    seq_t *coeff = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!coeff) return (seq_t)0;   /* allocation failure → 0 */
    coeff[0] = (seq_t)1;
    seq_t power = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t j = n; j >= k; --j) {
            coeff[j] = coeff[j] + seq_mul(coeff[j - k], power);
        }
        if (k < n) power = seq_mul(power, (seq_t)n);
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
#endif
}
