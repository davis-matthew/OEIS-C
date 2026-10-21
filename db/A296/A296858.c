/*
 * A296858  Numbers whose base-2 digits have #(pits) = #(peaks); see Comments.
 * https://oeis.org/A296858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A296858: numbers whose base-2 digits have equal number of pits and peaks. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;   /* next natural number to test */

/* bit length of a seq_t value (works for all supported types) */
static inline idx_t bitlen_seq(seq_t x) {
    idx_t len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* test whether a number has equal pits and peaks in its binary representation */
static inline bool equal_peaks_pits(seq_t x) {
    idx_t len = bitlen_seq(x);
    if (len < 3) return true;   /* no internal positions */

    int pits = 0, peaks = 0;
    for (idx_t j = 1; j <= len - 2; ++j) {
        seq_t prev = (x >> (unsigned)(j - 1)) & 1;
        seq_t cur  = (x >> (unsigned)j)       & 1;
        seq_t next = (x >> (unsigned)(j + 1)) & 1;

        if (prev > cur && cur < next) ++pits;
        else if (prev < cur && cur > next) ++peaks;
    }
    return pits == peaks;
}

/* resize the cache if necessary */
static inline void grow_cache(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* return the nth term (1‑based) of the sequence */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* guard; a(0) is undefined */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (equal_peaks_pits(cand)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
