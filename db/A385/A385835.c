/*
 * A385835  a(n) = 1 + Sum_{k=0..n-1} (1 + k^2) * a(k) * a(n-1-k).
 * https://oeis.org/A385835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 1 + Σ_{k=0}^{n-1} (1 + k^2) * a(k) * a(n-1-k)
 *            for n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system.  They are unsigned
   integer types; seq_t may be an arbitrary-precision class in C++.  */

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_size = 0;     /* number of terms currently stored */

/* Ensure that cache contains values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* If we already have enough terms, nothing to do */
    if (cache_size > 0 && n + 1 <= cache_size)
        return;

    /* Allocate a new array large enough for n+1 terms */
    seq_t *new_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failed */

    /* Copy existing terms into the new array */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* Compute any new terms required */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            new_cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t coeff = (seq_t)1 + ((seq_t)k * (seq_t)k);
            seq_t prod = coeff * new_cache[k] * new_cache[i - 1 - k];
            sum += prod;
        }
        new_cache[i] = (seq_t)1 + sum;
    }

    /* Replace the old cache with the new one */
    free(cache);
    cache = new_cache;
    cache_size = n + 1;
}

/* Public function as required */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
