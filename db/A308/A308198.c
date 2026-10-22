/*
 * A308198  Numbers m such that the tribonacci representation of m (A278038)
 *          ends in an odd number of 0's.
 * https://oeis.org/A308198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308198: Numbers m such that the tribonacci representation
   of m ends in an odd number of 0's.  The nth term is produced by
   testing successive integers and keeping those whose greedy tribonacci
   representation has a minimal used index that is odd. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    static seq_t *trib = NULL;
    static size_t trib_len = 0, trib_cap = 0;

    /* current integer to test */
    static seq_t next_m = 0;

    /* initialise caches on first call */
    if (!cache) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    if (!trib) {
        trib_cap = 8;
        trib = (seq_t*)malloc(trib_cap * sizeof(seq_t));
        trib[0] = (seq_t)0;
        trib[1] = (seq_t)1;
        trib[2] = (seq_t)1;
        trib_len = 3;
    }

    /* produce terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t m = next_m;
        ++next_m;

        /* extend trib array to cover m */
        while (trib_len > 0 && trib[trib_len-1] <= m) {
            if (trib_len == trib_cap) {
                trib_cap <<= 1;
                trib = (seq_t*)realloc(trib, trib_cap * sizeof(seq_t));
            }
            trib[trib_len] = trib[trib_len-1] + trib[trib_len-2] + trib[trib_len-3];
            ++trib_len;
        }

        /* accept 0 unconditionally */
        if (m == (seq_t)0) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = m;
            continue;
        }

        /* greedy tribonacci representation */
        idx_t min_idx = (idx_t)-1;
        seq_t rem = m;
        for (idx_t i = (idx_t)(trib_len-1); i > 0; --i) {
            if (trib[i] <= rem) {
                rem -= trib[i];
                if (i < min_idx) min_idx = i;
                if (rem == 0) break;
            }
        }

        /* odd minimal index -> accept */
        if ((min_idx & 1) != 0) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = m;
        }
    }

    return cache[n-1];
}
