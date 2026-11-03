/*
 * A333218  Numbers k such that the k-th composition in standard order is a
 *          permutation (of an initial interval).
 * https://oeis.org/A333218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by searching for k such that the k‑th composition in standard
   order is a permutation of an initial interval. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Determines whether k satisfies the property. */
static bool is_perm_k(idx_t k)
{
    if (k == 0) return true;          /* 0 is a valid term */

    /* Count number of bits of k. */
    int bits = 0;
    idx_t t = k;
    while (t) { bits++; t >>= 1; }

    /* Map a triangular number of bits to its m. */
    static int tri_to_m[65];
    static bool mapped = false;
    if (!mapped) {
        for (int i = 0; i < 65; ++i) tri_to_m[i] = 0;
        for (int m = 1; m <= 10; ++m) {                /* 10*11/2 = 55 <= 64 */
            int tri = m * (m + 1) / 2;
            tri_to_m[tri] = m;
        }
        mapped = true;
    }
    int m = tri_to_m[bits];
    if (m == 0) return false;          /* number of bits not triangular */

    /* Scan set bit positions (LSB first). */
    int used = 0;          /* bitmask for numbers 1..m (bit 0 unused) */
    int diff_count = 0;
    int prev = -1;         /* previous set bit index */
    int pos = 0;
    while (k) {
        if (k & 1U) {
            int d = pos - prev;                    /* d = j - prevj  */
            if (d > m || (used & (1 << d))) return false;
            used |= (1 << d);
            diff_count++;
            prev = pos;
        }
        k >>= 1;
        pos++;
    }
    return diff_count == m;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t last_k = 0;

    /* Initialise cache with first term 0 if empty. */
    if (cache_size == 0) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)0;
        cache_size = 1;
        last_k = 0;
    }

    /* Grow cache until it holds at least n terms. */
    while (cache_size < n) {
        idx_t k = last_k + 1;
        while (!is_perm_k(k)) ++k;
        /* Add new term. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = (seq_t)k;
        last_k = k;
    }

    return cache[(size_t)n - 1];
}
