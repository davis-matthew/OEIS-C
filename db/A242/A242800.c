/*
 * A242800  Numbers n such that (n^n+3)/(n+3) is an integer.
 * https://oeis.org/A242800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 2051, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers n such that (n^n+3)/(n+3) is an integer. */
/* Returns the n'th element of OEIS A242800 (offset 1). */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"   /* provides seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n) {
    /* Static cache of found terms.  On first call it is allocated. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  next_cand = 1;   /* next candidate to test */

    if (n == 0) return 0;                 /* undefined, but safe default */

    /* Grow cache when needed. */
    if (cache == NULL) {
        cap = 256;                        /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failed */
        len = 0;
        next_cand = 1;
    }

    /* Generate terms until we reach the requested index. */
    while (len < n) {
        seq_t mod = (seq_t)next_cand + 3;
        seq_t r   = mod_pow((seq_t)next_cand, (uint64_t)next_cand, mod);
        if ((r + 3) % mod == 0) {         /* satisfies the condition */
            if (len == cap) {             /* need more capacity */
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) return 0;       /* allocation failed */
                cache = tmp;
            }
            cache[len++] = (seq_t)next_cand;
        }
        ++next_cand;
    }

    return cache[n - 1];
}
