/*
 * A285162  1-limiting word of the morphism 0->10, 1-> 0011.
 * https://oeis.org/A285162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A285162 – 1-limiting word of the morphism 0->10, 1->0011.
   We compute it by iterating the derived morphism
   g(0) = 001110   (6 symbols)
   g(1) = 101000110011   (12 symbols)
   The initial word is 1, and the limit word is the fixed point
   of g()
   We cache the generated word up to the required index. */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure that cache has at least target terms. */
static void ensure_terms(idx_t target_idx)
{
    size_t target = (size_t)target_idx;
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 1;
        cache_len = 1;
    }
    while (cache_len < target) {
        /* compute new length */
        size_t new_len = 0;
        for (size_t i = 0; i < cache_len; ++i)
            new_len += (cache[i] == 0 ? 6 : 12);

        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return; /* allocation failed – leave cache unchanged */

        /* fill new_cache with expansions */
        size_t pos = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            seq_t v = cache[i];
            if (v == 0) {
                new_cache[pos++] = 0;
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
                new_cache[pos++] = 1;
                new_cache[pos++] = 1;
                new_cache[pos++] = 0;
            } else {
                new_cache[pos++] = 1;
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
                new_cache[pos++] = 0;
                new_cache[pos++] = 0;
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
                new_cache[pos++] = 1;
                new_cache[pos++] = 0;
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
                new_cache[pos++] = 1;
            }
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
