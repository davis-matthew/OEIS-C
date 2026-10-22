/*
 * A327106  BII-numbers of set-systems with maximum degree 2.
 * https://oeis.org/A327106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate BII‑numbers whose induced set‑system has maximum degree exactly 2 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* helper: test whether a candidate integer produces a set‑system with max degree 2 */
static bool valid_bii(idx_t cand) {
    size_t deg[64] = {0};          /* vertex degree counters, indices start at 0 (bit 1) */
    size_t maxdeg = 0;
    idx_t temp = cand;
    idx_t pos  = 1;                 /* current bit position of cand (1‑based) */

    while (temp) {
        if (temp & 1) {            /* this bit is a 1 */
            idx_t p = pos;
            idx_t t = p;
            idx_t bitpos = 1;      /* bit position inside p (1‑based) */

            while (t) {
                if (t & 1) {
                    size_t vidx = (size_t)(bitpos - 1);   /* vertex id */
                    ++deg[vidx];
                    if (deg[vidx] > 2) return false;     /* exceeds allowed degree */
                    if (deg[vidx] > maxdeg) maxdeg = deg[vidx];
                }
                t >>= 1;
                ++bitpos;
            }
        }
        temp >>= 1;
        ++pos;
    }
    return maxdeg == 2;            /* degree must be exactly 2 */
}

/* cache of found BII‑numbers, computed only once up to the required index */
static seq_t *cache      = NULL;   /* array holds the BII‑numbers */
static idx_t cache_cap   = 0;      /* allocated capacity of cache */
static idx_t cache_sz    = 0;      /* number of elements stored */
static idx_t last_checked = 0;     /* last candidate integer that was examined */

/* public routine for the sequence */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 for safety */

    /* grow the cache until we have at least n terms */
    while (cache_sz < n) {
        ++last_checked;            /* next candidate integer */

        if (valid_bii(last_checked)) {
            /* enlarge cache array if needed */
            if (cache_sz == cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_cache = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
                if (new_cache == NULL) abort();
                if (cache) memcpy(new_cache, cache, (size_t)cache_sz * sizeof(seq_t));
                if (cache) free((void*)cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
