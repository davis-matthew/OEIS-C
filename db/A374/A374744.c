/*
 * A374744  Numbers k such that the leaders of weakly decreasing runs in the
 *          k-th composition in standard order (A066099) are identical.
 * https://oeis.org/A374744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the leaders of weakly decreasing runs
   in the k‑th composition in standard order (A066099) are identical.
   The function a(n) returns the n‑th such number (offset 1).         */
#include <stdlib.h>
#include <stdint.h>

/* Check whether the leaders of all weakly decreasing runs in the
   composition of k (given by the standard mapping) are identical. */
static int passes(idx_t k)
{
    /* Positions of 1's in the reversed binary expansion of k,
       prepended with 0.  pos[0] = 0. */
    int pos[70];
    int pos_len = 1;           /* pos[0] already set */
    pos[0] = 0;

    idx_t tmp = k;
    int bitpos = 1;            /* count bits starting at 1 for LSB */
    while (tmp) {
        if (tmp & 1) {
            pos[pos_len++] = bitpos;
        }
        tmp >>= 1;
        bitpos++;
    }

    /* First differences to get parts, then reverse to obtain composition. */
    int parts[70];
    int parts_len = 0;
    for (int i = 1; i < pos_len; ++i) {
        parts[parts_len++] = pos[i] - pos[i - 1];
    }

    /* Reverse the parts array. */
    for (int i = 0; i < parts_len / 2; ++i) {
        int t = parts[i];
        parts[i] = parts[parts_len - 1 - i];
        parts[parts_len - 1 - i] = t;
    }

    /* Empty composition (k == 0) passes automatically. */
    if (parts_len == 0)
        return 1;

    /* Verify that all run leaders are the same. */
    int expected = parts[0];
    for (int i = 1; i < parts_len; ++i) {
        if (parts[i] > parts[i - 1]) {   /* start of a new run */
            if (parts[i] != expected)
                return 0;
        }
    }
    return 1;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)            /* (not used; safe guard) */
        return 0;

    /* Static state for memoisation. */
    static size_t cache_size = 0;   /* how many terms we have */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static seq_t *cache = NULL;      /* array of found k's */
    static idx_t next_k     = 0;    /* next integer k to test */

    /* Grow the cache if needed. */
    while (cache_size < (size_t)n) {
        /* Find next passing k. */
        while (!passes(next_k))
            ++next_k;

        /* Store it. */
        if (cache_size >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();           /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = (seq_t)next_k;
        ++next_k;                /* advance to next candidate for future */
    }

    return cache[n - 1];
}
