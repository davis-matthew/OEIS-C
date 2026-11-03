/*
 * A384137  a(n) = (4*n)!/(3*n)! * Sum_{k=1..n} 1/(3*n+k).
 * https://oeis.org/A384137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n)!/(3*n)! * Sum_{k=1..n} 1/(3*n+k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute each term of the sum as the product of all numbers
   in the range [3*n+1, 4*n] except one, using prefix and suffix products. */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 for the sequence offset, but handle n==0 defensively */
    if (n == 0) return 0;

    const idx_t m = n;          /* number of terms in the sum */
    const seq_t start = (seq_t)3 * n + 1U;  /* first integer in the product range */

    /* Allocate prefix and suffix product arrays */
    seq_t *pref = mem_new(seq_t, (m + 1));
    seq_t *suf  = mem_new(seq_t, (m + 1));
    if (!pref || !suf) return 0; /* memory failure fallback */

    /* Prefix product: pref[i] = product of the first i numbers in the range */
    pref[0] = 1;
    for (idx_t i = 0; i < m; ++i) {
        pref[i + 1] = pref[i] * (start + (seq_t)i);
    }

    /* Suffix product: suf[i] = product of numbers from i to m-1 in the range */
    suf[m] = 1;
    for (idx_t i = m; i > 0; --i) {
        suf[i - 1] = (start + (seq_t)(i - 1)) * suf[i];
    }

    /* Sum the terms: each term excludes the element at position k-1 */
    seq_t result = 0;
    for (idx_t k = 1; k <= m; ++k) {
        result += pref[k - 1] * suf[k];
    }

    mem_free(pref);
    mem_free(suf);
    return result;
}
