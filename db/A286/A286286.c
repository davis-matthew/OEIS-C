/*
 * A286286  a(0) = 0; thereafter, a(n) = (2*n-1)*a(n-1) + 1.
 * https://oeis.org/A286286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; thereafter a(n) = (2*n-1)*a(n-1) + 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t next   = 1;

    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        next = 1;
    }

    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap = new_cap;
    }

    for (idx_t i = next; i <= n; ++i) {
        cache[i] = ((seq_t)(2*i-1) * cache[i-1]) + 1;
    }
    next = n + 1;

    return cache[n];
}
