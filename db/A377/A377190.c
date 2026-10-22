/*
 * A377190  Expansion of 1/(1 - 4*x^2 - 4*x^3)^(7/2).
 * https://oeis.org/A377190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*(n+5)*a(n-2) + 2*(2*n+15)*a(n-3))/n with a(0) = 1,
 *            a(1) = 0, a(2) = 14
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Ensure that the cache can hold indices 0..n (inclusive). Newly
   allocated slots are zeroed. */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    size_t cap = *cap_ptr;
    if (n < cap) return;

    size_t new_cap = (cap == 0) ? 1 : cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;          /* double until > n */

    seq_t *new_cache = (seq_t *)realloc(*cache_ptr,
                     new_cap * sizeof(seq_t));
    if (!new_cache) abort();                            /* memory exhausted */

    /* zero the newly allocated part */
    for (size_t j = cap; j < new_cap; ++j) new_cache[j] = 0;

    *cache_ptr = new_cache;
    *cap_ptr  = new_cap;
}

/* Return the n-th term of OEIS sequence A377190. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Make sure the cache can store indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base cases – redefining them is harmless. */
    cache[0] = (seq_t)1;
    if (n >= 1) cache[1] = (seq_t)0;
    if (n >= 2) cache[2] = (seq_t)14;

    /* Compute values from 3 up to n. */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* Skip if already computed. */
        if (cache[i] != 0) continue;

        seq_t term1 = (seq_t)4 * ((seq_t)(i + (idx_t)5)) * cache[i - 2];
        seq_t term2 = (seq_t)2 * ((seq_t)(2 * (seq_t)i + (seq_t)15)) * cache[i - 3];
        cache[i] = (term1 + term2) / ((seq_t)i);
    }

    return cache[n];
}
