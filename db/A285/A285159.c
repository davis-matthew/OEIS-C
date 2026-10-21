/*
 * A285159  0-limiting word of the morphism 0->10, 1-> 0011.
 * https://oeis.org/A285159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0‑limiting word of the morphism 0→10, 1→0011.
    The function builds the even‑iteration word of the morphism until
    it contains index n and returns the n‑th symbol (1‑based). */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache_even = NULL;   /* word of the latest even iteration   */
static size_t cache_even_len = 0;  /* length of cache_even                */

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0;

    /* Initialise the cache with the first even iteration (iteration 2). */
    if (cache_even == NULL) {
        /* w2 = "001110" : expansion of "10"  (iteration 1 → 2). */
        seq_t *tmp = (seq_t *)malloc(6 * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;  /* memory allocation failed */
        size_t j = 0;
        /* 1 → 0011 */
        tmp[j++] = (seq_t)0;
        tmp[j++] = (seq_t)0;
        tmp[j++] = (seq_t)1;
        tmp[j++] = (seq_t)1;
        /* 0 → 10   */
        tmp[j++] = (seq_t)1;
        tmp[j++] = (seq_t)0;
        cache_even = tmp;
        cache_even_len = 6;
    }

    /* Build larger even‑iteration words until they cover index n. */
    while (cache_even_len < (size_t)n) {
        /* First expansion: even → odd (i.e., length × 3). */
        size_t len_odd = cache_even_len * 3;
        seq_t *odd = (seq_t *)malloc(len_odd * sizeof(seq_t));
        if (odd == NULL)
            return (seq_t)0;  /* memory allocation failed */
        size_t pos = 0;
        for (size_t i = 0; i < cache_even_len; ++i) {
            seq_t sym = cache_even[i];
            if (sym == (seq_t)0) {          /* 0 → 10 */
                odd[pos++] = (seq_t)1;
                odd[pos++] = (seq_t)0;
            } else {                       /* 1 → 0011 */
                odd[pos++] = (seq_t)0;
                odd[pos++] = (seq_t)0;
                odd[pos++] = (seq_t)1;
                odd[pos++] = (seq_t)1;
            }
        }

        /* Second expansion: odd → next even (length × 3 again). */
        size_t new_len = len_odd * 3;
        seq_t *new_even = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_even == NULL) {
            free(odd);
            return (seq_t)0;  /* memory allocation failed */
        }
        pos = 0;
        for (size_t i = 0; i < len_odd; ++i) {
            seq_t sym = odd[i];
            if (sym == (seq_t)0) {          /* 0 → 10 */
                new_even[pos++] = (seq_t)1;
                new_even[pos++] = (seq_t)0;
            } else {                       /* 1 → 0011 */
                new_even[pos++] = (seq_t)0;
                new_even[pos++] = (seq_t)0;
                new_even[pos++] = (seq_t)1;
                new_even[pos++] = (seq_t)1;
            }
        }

        /* Update cache. */
        free(cache_even);
        free(odd);
        cache_even = new_even;
        cache_even_len = new_len;
    }

    return cache_even[(size_t)n - 1];
}
