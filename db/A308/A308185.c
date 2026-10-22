/*
 * A308185  Fixed point (beginning with a) of the morphism a -> abab, b -> b,
 *          over the alphabet {a,b} = {0,1}.
 * https://oeis.org/A308185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth term of the infinite fixed point of the morphism
 *            a → a b a b, b → b (with 0 representing a and 1 representing b).
 *            The sequence is defined by the recursive word construction
 *            described above
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array holding the current generated prefix */
static idx_t cache_len = 0;       /* number of elements stored in cache */

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    /* initial cache for the single starting symbol 'a' (0) */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* fallback on allocation failure */
        cache[0] = 0;
        cache_len = 1;
    }

    /* extend the cache until it contains at least n elements */
    while (cache_len < n) {
        /* compute length of the next expansion */
        idx_t new_len = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            new_len += (cache[i] == 0) ? 4 : 1;  /* a → 4 chars, b → 1 char */
        }

        /* allocate space for the expanded sequence */
        seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * new_len);
        if (!new_cache) return (seq_t)0;  /* allocation failure */

        /* perform the expansion */
        idx_t pos = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == 0) {               /* 'a' → a b a b  (0 1 0 1) */
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
            } else {                           /* 'b' → b          (1) */
                new_cache[pos++] = 1;
            }
        }

        /* replace old cache with the new one */
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    /* return the requested element (sequence is 0-indexed internally) */
    return cache[n - 1];
}
