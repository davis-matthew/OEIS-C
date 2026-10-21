/*
 * A260025  Convert the last term from decimal to binary! a(1)=4.
 * https://oeis.org/A260025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 4; a(n) = decimal_of_binary(a(n‑1))
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_size = 0;    /* number of elements stored */
static size_t cache_cap  = 0;    /* allocated capacity */

/* Compute the next term from the previous one */
static seq_t next_term(const seq_t prev)
{
    unsigned len = bitlen_seq_t(prev);
    seq_t cur = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        seq_t bit = (prev >> i) & (seq_t)1;
        cur = cur * (seq_t)10 + bit;
    }
    return cur;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset = 1, but guard zero */

    /* Ensure the cache covers up to n */
    while (cache_size < n) {
        if (cache_size == 0) {        /* initialise first term */
            cache = (seq_t *)malloc(sizeof(seq_t));
            if (!cache) abort();
            cache_cap = 1;
            cache[0] = (seq_t)4;
            cache_size = 1;
            continue;
        }
        /* grow cache if necessary */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        cache[cache_size++] = next_term(cache[cache_size - 1]);
    }
    return cache[n - 1];
}
