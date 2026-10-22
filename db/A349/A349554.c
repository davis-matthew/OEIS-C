/*
 * A349554  a(n) = A054108(n) + 4*(-1)^n.
 * https://oeis.org/A349554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2*(n+1), n+1) - a(n-1), a(1)=1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

#define MAX_N 10000
/* Cache of already computed terms.  Index 0 is unused. */
static seq_t cache[MAX_N + 1];
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    /* Ensure the first term is initialized. */
    if (max_computed == 0) {
        cache[1] = 1;
        max_computed = 1;
    }

    /* If the required index is larger than the preallocated cache,
       compute directly without storing past MAX_N.  This case
       should not occur for the typical usage (n ≤ 10000). */
    if (n > MAX_N) {
        seq_t prev = 1;                 /* a(1) */
        for (idx_t k = 2; k <= n; ++k) {
            seq_t bin = binom((idx_t)(2 * (k + 1)), (idx_t)(k + 1));
            seq_t cur = bin - prev;
            prev = cur;
        }
        return prev;
    }

    /* Compute new terms up to n, reusing the cached values. */
    if (n > max_computed) {
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            seq_t bin = binom((idx_t)(2 * (k + 1)), (idx_t)(k + 1));
            cache[k] = bin - cache[k - 1];
        }
        max_computed = n;
    }

    return cache[n];
}
