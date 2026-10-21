/*
 * A296691  Numbers whose base-3 digits d(m), d(m-1), ... d(0) have #(rises) =
 *          #(falls); see Comments.
 * https://oeis.org/A296691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) are numbers whose base‑3 digits have equal rises and falls
 * (computed by brute‑force search with caching). */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Return true if v has an equal number of rises and falls in its base‑3
 * representation.  A rise occurs when a digit is smaller than the previous
 * (more significant) digit; a fall occurs when it is larger. */
static bool ok(seq_t v) {
    /* A single‑digit number has no rises or falls. */
    if (v == (seq_t)0)
        return true;

    seq_t rem = v % (seq_t)3;
    v        = v / (seq_t)3;
    int rises = 0, falls = 0;

    while (v > (seq_t)0) {
        seq_t d = v % (seq_t)3;
        v       = v / (seq_t)3;
        if (d < rem)
            rises++;
        else if (d > rem)
            falls++;
        rem = d;
    }
    return rises == falls;
}

/* Return the nth term (1‑based) of A296691.  The function maintains a
 * static cache of previously found terms so it works efficiently even if
 * calls are in random order. */
seq_t a(idx_t n) {
    if (n == (idx_t)0)          /* index 0 is not used by the OEIS entry */
        return (seq_t)0;

    /* Static cache state */
    static seq_t *cache   = NULL;
    static size_t clen    = 0;   /* number of values stored in cache   */
    static size_t ccap    = 0;   /* current capacity of cache array    */
    static seq_t next     = (seq_t)0; /* next integer candidate to test */

    /* Expand the cache until it holds at least n terms. */
    while (clen < (size_t)n) {
        /* Find the next integer that satisfies the condition. */
        do {
            next++;
        } while (!ok(next));

        /* Grow the cache array if needed. */
        if (clen == ccap) {
            size_t newcap = ccap ? ccap * 2 : 16;
            seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
            if (!newcache)          /* allocation failed */
                abort();
            /* Copy existing elements to the new cache. */
            for (size_t i = 0; i < clen; i++)
                newcache[i] = cache[i];
            if (cache)
                free(cache);       /* free old cache */
            cache = newcache;
            ccap  = newcap;
        }

        /* Store the found value. */
        cache[clen++] = next;
    }

    return cache[(size_t)n - 1];
}
