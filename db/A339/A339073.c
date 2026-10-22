/*
 * A339073  Number of strings of Hebrew letters with a gematria value equal to
 *          n.
 * https://oeis.org/A339073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = sum_{s∈{1,2,…,9,10,20,…,90,100,200,300,400}} a(n−s) *
 *            with a(0)=1. a(1) is the first term in the OEIS listing
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>    /* malloc, realloc */

static const idx_t VALUES[] = {
    1,2,3,4,5,6,7,8,9,      /* 1‑9 */
    10,                     /* 10 */
    20,30,40,50,60,70,80,90,/* 20‑90 step 10 */
    100,200,300,400         /* 100‑400 step 100 */
};
static const size_t N_VALUES = sizeof(VALUES) / sizeof(VALUES[0]);

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* initialise cache (a(0) = 1) */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);          /* out of memory */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        len = 1;
    }

    /* grow capacity if required */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                 /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing values up to n */
    if ((size_t)n >= len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < N_VALUES; ++k) {
                idx_t v = VALUES[k];
                if (i >= v) sum += cache[i - v];
            }
            cache[i] = sum;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
