/*
 * A355321  Numbers k such that the k-th composition in standard order has the
 *          same number of even parts as odd.
 * https://oeis.org/A355321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th non‑negative integer k such that the k-th
 *            composition (in graded reverse‑lexicographic order) has the same
 *            number of even parts as odd parts
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: test if k satisfies the parity condition */
static inline bool good_k(idx_t k) {
    idx_t prev = 0;          /* previous set‑bit position (1‑based) */
    int even = 0, odd = 0;   /* counters for even/odd parts */
    idx_t kk   = k;
    idx_t pos  = 1;          /* current bit position (1‑based) */

    while (kk) {
        if (kk & 1) {                     /* bit set at this position */
            idx_t part = pos - prev;      /* part size = difference of positions */
            if (part & 1) odd++; else even++;
            prev = pos;
        }
        kk >>= 1;
        ++pos;
    }
    /* For k = 0 the loop is skipped; even==odd==0, which counts. */
    return even == odd;
}

/* Grow the cache array when needed */
static void grow_cache(size_t *cap, seq_t **arr) {
    size_t new_cap = (*cap) ? (*cap) * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* run‑time failure is acceptable */
    *arr   = tmp;
    *cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 0;           /* next k to test */

    if (n == 0) return (seq_t)0;       /* undefined but safe */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* find the next good k */
        while (!good_k(next_k)) ++next_k;

        /* store it */
        if (cache_len == cache_cap) grow_cache(&cache_cap, &cache);
        cache[cache_len++] = (seq_t)next_k;

        ++next_k;
    }

    return cache[n - 1];
}
