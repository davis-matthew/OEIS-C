/*
 * A307612  Partial sums of the permutation A307485: one odd, two even, four
 *          odd, eight even, etc.
 * https://oeis.org/A307612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of the permutation A307485: one odd, two even, four odd, eight even, etc. */
#include <stdlib.h>   /* malloc, realloc, free */
#include "../utils/mem.h"

/* Return the permutation element at position pos (0‑based). */
static seq_t perm_at(idx_t pos)
{
    /* block index k = floor(log2(pos+1)) */
    idx_t k = 0;
    idx_t t = pos + 1;
    while (t > 1) {
        t >>= 1;
        ++k;
    }

    /* counts of odds/evens used before block k */
    seq_t odds_before = 0;
    seq_t evens_before = 0;
    for (idx_t i = 0; i < k; ++i) {
        idx_t size_idx = (idx_t)1 << i;               /* block size 2^i as idx_t */
        seq_t size = (seq_t)size_idx;                /* cast to seq_t */
        if ((i & 1) == 0)                            /* odd block */
            odds_before += size;
        else                                         /* even block */
            evens_before += size;
    }

    /* offset inside the block */
    idx_t offset = pos - (((idx_t)1 << k) - 1);

    /* first element of the block */
    seq_t first;
    if ((k & 1) == 0)                                /* odd block */
        first = (seq_t)2 * odds_before + 1;
    else                                             /* even block */
        first = (seq_t)2 * evens_before + 2;

    /* element value */
    seq_t off_seq = (seq_t)offset;
    return first + (seq_t)2 * off_seq;
}

static seq_t *cache = NULL;
static idx_t  cache_cap = 0;   /* allocated length */
static idx_t  cache_size = 0;  /* largest n computed, cache[0] = 0 */

/* Return a(n): partial sum of the first n elements of the permutation. */
seq_t a(idx_t n)
{
    /* initial allocation */
    if (cache == NULL) {
        cache_cap = 1;
        cache = mem_new(seq_t, (size_t)cache_cap);
        if (cache == NULL) exit(1);          /* out of memory */
        cache[0] = 0;
        cache_size = 0;
    }

    /* expand cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)cache_cap ? (size_t)cache_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        if (new_cache == NULL) exit(1);
        cache = new_cache;
        cache_cap = (idx_t)new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t perm = perm_at(i - 1);         /* term at position i-1 */
        cache[i] = cache[i - 1] + perm;
    }
    cache_size = n;

    return cache[n];
}
