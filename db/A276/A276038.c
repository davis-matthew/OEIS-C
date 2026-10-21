/*
 * A276038  Numbers n such that product of digits of n is a power of 6.
 * https://oeis.org/A276038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1395, u32 n <= 8441, u64 n <= 8441,
 *            u128 n <= 8441, big any n
 * Helpers:   digit_product, is_pow6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose digit product is a power of 6 */

/* Digit product helper provided by the test harness. */
#include "../utils/digit_product.h"
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_pow6.h"

/* Cache of discovered terms. */
static seq_t *cache = NULL;      /* array of found terms */
static idx_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t next_candidate = (seq_t)1; /* next integer to examine */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < (size_t)needed)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        return; /* allocation failed; caller will handle */

    if (cache != NULL) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* External function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for offset 1, safe fallback */

    /* Generate terms until we have at least n elements. */
    while (cache_len < n) {
        seq_t prod = digit_product(next_candidate);
        if (is_pow6(prod)) {
            ensure_cache_cap(cache_len + 1);
            if (cache != NULL)  /* check after ensuring allocation */
                cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
