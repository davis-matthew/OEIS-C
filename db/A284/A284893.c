/*
 * A284893  Fixed point of the morphism 0 -> 01, 1 -> 0111.
 * https://oeis.org/A284893
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

/* Sequence: a(n) is the n‑th symbol (0 or 1) of the fixed point
 * of the morphism 0 → 01, 1 → 0111.
 * The implementation expands the word iteratively until
 * the requested index lies inside the generated part.
 */

#include <stdlib.h>      /* malloc, free */

/* static cache holding the generated prefix of the fixed point */
static seq_t *cache = NULL;   /* array of 0/1 values */
static idx_t cache_len = 0;   /* current length of the array */

/* Ensure that cache contains at least indices 0 .. n-1.
 * n is the desired length (>=1). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;                 /* nothing to ensure (used for low indices) */

    /* initialise the empty cache with the first word "0" */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = 0;
    }

    /* iteratively expand until length is at least n */
    while (cache_len < n) {
        idx_t oldlen = cache_len;
        idx_t newlen = 0;

        /* first count the length of the next level */
        for (idx_t i = 0; i < oldlen; ++i) {
            newlen += (cache[i] == 0 ? 2 : 4);
        }

        /* allocate buffer for the next level */
        seq_t *newbuf = (seq_t *)malloc(sizeof(seq_t) * newlen);
        if (newbuf == NULL) exit(1);

        /* expand each symbol */
        idx_t pos = 0;
        for (idx_t i = 0; i < oldlen; ++i) {
            seq_t val = cache[i];
            if (val == 0) {           /* 0 → 01 */
                newbuf[pos++] = 0;
                newbuf[pos++] = 1;
            } else {                  /* 1 → 0111 */
                newbuf[pos++] = 0;
                newbuf[pos++] = 1;
                newbuf[pos++] = 1;
                newbuf[pos++] = 1;
            }
        }

        /* replace old cache with new buffer */
        free(cache);
        cache     = newbuf;
        cache_len = newlen;
    }
}

/* Return the n-th term of sequence A284893 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* index below offset is undefined, return 0 */
    ensure_cache(n);              /* needs at least n symbols (indices 0..n-1) */
    return cache[n - 1];
}
