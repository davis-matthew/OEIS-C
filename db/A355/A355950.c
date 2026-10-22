/*
 * A355950  a(n) = Sum_{k=1..n} k^(k-1) * floor(n/k).
 * https://oeis.org/A355950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k^(k‑1) * ⌊n/k⌋
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses a cache of k^(k‑1) values to keep the complexity O(n). */

#include <stdlib.h>
#include "../utils/seq_pow.h"

/* Cache of powers k^(k‑1); index 0 is unused. */
static seq_t *pow_cache = NULL;
static idx_t pow_cap   = 0;

/* Grow the cache so that values up to n are available. */
static void ensure_powers(idx_t n)
{
    if (n <= pow_cap) return;
    seq_t *new_cache = (seq_t *)realloc(pow_cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);           /* OOM */
    for (idx_t k = pow_cap + 1; k <= n; ++k)
        new_cache[k] = seq_pow((seq_t)k, k - 1);
    pow_cache = new_cache;
    pow_cap   = n;
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* OEIS offset: a(1) is first term */
    ensure_powers(n);
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += pow_cache[k] * (seq_t)(n / k);
    }
    return sum;
}
